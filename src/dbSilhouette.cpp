#define USE_R_RGN

#include "datatypes.h"
#include "matrix.hpp"
#include "vector.hpp"
#include "functional_data_mcmc.hpp"


#include <Rcpp.h>

/***
 *
 * y in R^(n x d). each row, y_i, is an observation; y_i of dimension d
 * theta in R^(k x T x (d x 2)). each row is an MCMC sample; each theta_i is a vector of dimension k; each (theta_i)_l=(mean,variance)
 * pi in N^n. partition estimator
***/

template<class dataModel>
Rcpp::NumericVector dbSilhouette(const _matrix<double> &y, const _vector<_vector<_matrix<double> > > &theta, const _vector<unsigned int> pi) {
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
  for (unsigned int i=0; i<dbs.size(); i++) {
    dbs[i]=0.0;
  }

  unsigned int m0, m1, k;
  double num, den;

  for (unsigned int t=0; t<theta.size(); t++) {

    for (unsigned int i=0; i<y.size(); i++) {
      m0= pi[i]-pi0;

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
      num= log(nj[m0]) + dm.logK(i,theta[t][m0]) - tauM;

      den= abs(log(nj[m0]) + dm.logK(0,theta[t][m0]) - log(nj[m1]) - dm.logK(0,theta[t][m1]));
      for (unsigned int l=1; l<y.size(); l++) {
        double maxTmp= abs(log(nj[m0]) + dm.logK(l,theta[t][m0]) - log(nj[m1]) - dm.logK(l,theta[t][m1]));
        if (maxTmp>den) {
          den= maxTmp;
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



inline unsigned int idx(unsigned int k, unsigned int t, unsigned int r, unsigned int c, unsigned int K, unsigned int T, unsigned int R) {
  return k + t*K + r*K*T + c*R*K*T;
}

/***
 * data
 * postPartitionMCMC
 *  $d. vector of unique values
 *  $f. frequency
 *  $p. partition
 *  $o. indicators
 *  $th. parameters
 ***/
// [[Rcpp::export]]
Rcpp::NumericVector dbSilhouetteCpp(
  Rcpp::NumericMatrix data,
  Rcpp::List postPartitionMCMC
) {
  _matrix<double> y(data.rows(),data.cols());
  _vector<_vector<_matrix<double> > > theta;
  _vector<unsigned int> pi(data.rows());

  for (unsigned int i=0; i<y.rows(); i++) {
    for (unsigned int j=0; j<y.cols(); j++) {
      y(i,j)= data(i,j);
    }
  }

  unsigned int T, K, R, C;

  Rcpp::NumericVector th, tmpV;

  tmpV= postPartitionMCMC["d"];
  th= postPartitionMCMC["th"];

  // th.size() = K x T x R x C
  T= postPartitionMCMC["f"];
  K= tmpV.size();
  R= data.cols();
  C= th.size()/(T*K*R);

  theta.resize(T);
  for (unsigned int t=0; t<T; t++) {
    theta[t].resize(K);

    for (unsigned int j=0; j<K; j++) {
      theta[t][j].resize(R,C);

      for (unsigned int r=0; r<R; r++) {
        for (unsigned int c=0; c<C; c++) {
          theta[t][j](r,c)= th[idx(j,t,r,c,K,T,R)];
        }
      }
    }
  }

  tmpV= postPartitionMCMC["p"];
  for (unsigned int i=0; i<pi.size(); i++) {
    pi[i]= tmpV[i];
  }

  return dbSilhouette<datamodel>(y,theta,pi);
}

