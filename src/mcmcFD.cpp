#define USE_R_RGN

#include "datatypes.h"
#include "matrix.hpp"
#include "vector.hpp"
#include "functional_data_mcmc.hpp"
#include "functional_data_missing_mcmc.hpp"

#include <Rcpp.h>

// [[Rcpp::export]]
Rcpp::List mcmcFunctionalDataCpp(
  Rcpp::NumericMatrix data,
  Rcpp::NumericMatrix baseMeasure,
  double r,
  Rcpp::NumericVector hyperWeights,
  Rcpp::NumericVector iterations,
  double p0= 0.5, // initial weight value
  unsigned int lag= 1,
  unsigned int n0= 10
) {
  // c++ data storage of R objects
	datamodel dm;
	// datamodel::parameter bParam;

  unsigned int i, j;
	datamodel::data y(data.rows(),data.cols());

  for (i=0; i<data.rows(); i++) {
    for (j=0; j<data.cols(); j++) {
      y(i,j)= data(i,j);
    }
  }
  dm.setData(y);

  if (baseMeasure.rows()==data.cols()) {
    _vector<double> vm0(data.cols()), vc0(data.cols()), va0(data.cols()), vb0(data.cols());
		for (unsigned int l=0; l<data.cols(); l++) {
			vm0[l]= baseMeasure(l,0);
			vc0[l]= baseMeasure(l,1);
			va0[l]= baseMeasure(l,2);
			vb0[l]= baseMeasure(l,3);
		}
		dm.setBaseMeasureParameters(vm0,vc0,va0,vb0);
  } else {
     // same base measure for all recorded times
    dm.setBaseMeasureParameters(baseMeasure(0,0),baseMeasure(0,1),baseMeasure(0,2),baseMeasure(0,3));
  }

	functional_data_mcmc mcmc(dm);

  _vector<double> hParam(4);
  hParam[0]= p0;
  hParam[1]= r;
  hParam[2]= hyperWeights[0];
  hParam[3]= hyperWeights[1];
	mcmc.setHyperParameters(hParam);

	mcmc.run(iterations[0],iterations[1],lag,n0);

  // theta, d, N, Nmax, pp, k, upsilon
  Rcpp::NumericMatrix th;
  Rcpp::IntegerMatrix pp, d, N;
  Rcpp::IntegerVector Nmax, k;
  Rcpp::NumericVector l;

  mcmc.saveR(th, d, N, Nmax, pp, k, l);

  return Rcpp::List::create(
      Rcpp::Named("th")= th,
      Rcpp::Named("d")= d,
      Rcpp::Named("N")= N,
      Rcpp::Named("Nmax")= Nmax,
      Rcpp::Named("p")= pp,
      Rcpp::Named("k")= k,
      Rcpp::Named("l")= l
      );
}

// [[Rcpp::export]]
Rcpp::List mcmcFunctionalDataMissingCpp(
  Rcpp::NumericMatrix data,
  Rcpp::NumericMatrix baseMeasure,
  double r,
  Rcpp::NumericVector hyperWeights,
  Rcpp::NumericVector iterations,
  double p0= 0.5, // initial weight value
  unsigned int lag= 1,
  unsigned int n0= 10
) {

	datamodelMissing dm;

  unsigned int i, j;
	datamodelMissing::data y(data.rows(),data.cols());

  for (i=0; i<data.rows(); i++) {
    for (j=0; j<data.cols(); j++) {
      if (Rcpp::traits::is_nan<REALSXP>(data(i,j)) || Rcpp::NumericVector::is_na(data(i,j))) {
        y(i,j)= nan("");
      } else {
        y(i,j)= data(i,j);
      }
    }
  }
  dm.setData(y);

  if (baseMeasure.rows()==data.cols()) {
    _vector<double> vm0(data.cols()), vc0(data.cols()), va0(data.cols()), vb0(data.cols());
		for (unsigned int l=0; l<data.cols(); l++) {
			vm0[l]= baseMeasure(l,0);
			vc0[l]= baseMeasure(l,1);
			va0[l]= baseMeasure(l,2);
			vb0[l]= baseMeasure(l,3);
		}
		dm.setBaseMeasureParameters(vm0,vc0,va0,vb0);
  } else {
     // same base measure for all recorded times
    dm.setBaseMeasureParameters(baseMeasure(0,0),baseMeasure(0,1),baseMeasure(0,2),baseMeasure(0,3));
  }

	functional_data_missing_mcmc mcmc(dm);

  _vector<double> hParam(4);
  hParam[0]= p0;
  hParam[1]= r;
  hParam[2]= hyperWeights[0];
  hParam[3]= hyperWeights[1];
	mcmc.setHyperParameters(hParam);

	mcmc.run(iterations[0],iterations[1],lag,n0);

  // theta, d, N, Nmax, pp, k, upsilon
  Rcpp::NumericMatrix th;
  Rcpp::IntegerMatrix pp, d, N;
  Rcpp::IntegerVector Nmax, k;
  Rcpp::NumericVector l;
  Rcpp::IntegerMatrix iM;
  Rcpp::NumericMatrix yM;

  mcmc.saveR(th, d, N, Nmax, pp, k, l, iM, yM);

  return Rcpp::List::create(
      Rcpp::Named("th")= th,
      Rcpp::Named("d")= d,
      Rcpp::Named("N")= N,
      Rcpp::Named("Nmax")= Nmax,
      Rcpp::Named("p")= pp,
      Rcpp::Named("k")= k,
      Rcpp::Named("l")= l,
      Rcpp::Named("iM")= iM,
      Rcpp::Named("yM")=yM
      );
}

