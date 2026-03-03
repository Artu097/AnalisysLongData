#include "datatypes.h"
#include "matrix.hpp"
#include "vector.hpp"
#include "functional/functional_data_mcmc.hpp"

#include <Rcpp.h>
/***
 *
 * y in R^(n x d). each row, y_i, is an observation; y_i of dimension d
 * theta in R^(k x T x (d x 2)). each row is an MCMC sample; each theta_i is a vector of dimension k; each (theta_i)_l=(mean,variance)
 * pi in N^n. partition estimator
 ***/

template<class dataModel>
Rcpp::NumericVector dbSilhouetteQuotient(const _matrix<double> &y, const _vector<_vector<_matrix<double> > > &theta, const _vector<unsigned int> pi) {
  dataModel dm;
  dm.setData(datamodel::data(y));

  _vector<unsigned int> nj(theta[0].size());
  unsigned int pi0= pi.min();

  for (unsigned int i=0; i<nj.size(); i++) {
    nj[i]=0;
  }
  for (unsigned int i=0; i<pi.size(); i++) {
    nj[pi[i]-pi0]++;
  }

  Rcpp::NumericVector dbs(y.rows());
  // for (unsigned int i=0; i<dbs.size(); i++) {
  //   dbs[i]=0.0;
  // }

  unsigned int m0, m1, k;
  _vector<double> num(theta.size()), den(theta.size());

  for (unsigned int i=0; i<y.size(); i++) {
    m0= pi[i]-pi0;
    dbs[i]= 0.0;

    for (unsigned int t=0; t<theta.size(); t++) {
      if (m0==0) {
        m1= 1;
        k= 2;
      } else {
        m1= 0;
        k= 1;
      }
      double tauM= log(nj[m1]) + dm.logK(i,theta[t][m1]);
      for (; k<theta[t].size(); k++) {
        if (k==m0) continue;
        double tauTmp= log(nj[k]) + dm.logK(i,theta[t][k]);
        if (tauTmp>tauM) {
          tauM= tauTmp;
          m1= k;
        }
      }
      num[t]= log(nj[m0]) + dm.logK(i,theta[t][m0]) - tauM;

      den[t]= abs(log(nj[m0]) + dm.logK(0,theta[t][m0]) - log(nj[m1]) - dm.logK(0,theta[t][m1]));
      for (unsigned int l=1; l<y.size(); l++) {
        double maxTmp= abs(log(nj[m0]) + dm.logK(l,theta[t][m0]) - log(nj[m1]) - dm.logK(l,theta[t][m1]));
        if (maxTmp>den[t]) {
          den[t]= maxTmp;
        }
      }

      dbs[i]+= num/den;
    }

  }

  for (unsigned int i=0; i<y.rows(); i++) {
    dbs[i]/= theta.size();
  }

  return dbs;
}

