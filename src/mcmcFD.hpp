#ifndef MCMCFD_HPP
#define MCMCFD_HPP

#include <Rcpp.h>

Rcpp::List mcmcFunctionalDataCpp(Rcpp::NumericMatrix data, Rcpp::NumericMatrix baseMeasure, double r, Rcpp::NumericVector hyperWeights, Rcpp::NumericVector iterations, double p0, unsigned int lag, unsigned int n0);

Rcpp::List mcmcFunctionalDataMissingCpp(Rcpp::NumericMatrix data, Rcpp::NumericMatrix baseMeasure, double r, Rcpp::NumericVector hyperWeights, Rcpp::NumericVector iterations, double p0, unsigned int lag, unsigned int n0);

#endif
