#define USE_R_RGN
#include <Rcpp.h>
#include "dbSilhouette.hpp"

// 1. Cabecera src
#include "src_modulo.hpp"
#include "mcmcFD.hpp"

using namespace Rcpp;

// 3. Exportación y cambio de nombres
// [[Rcpp::export]]
Rcpp::List bridge_mcmcFunctionalData(Rcpp::NumericMatrix data, Rcpp::NumericMatrix baseMeasure, double r, Rcpp::NumericVector hyperWeights, Rcpp::NumericVector iterations, double p0 = 0.5, unsigned int lag = 1, unsigned int n0 = 10) {
  // Llama a la función original dentro de mcmcFD.cpp
  return mcmcFunctionalDataCpp(data, baseMeasure, r, hyperWeights, iterations, p0, lag, n0);
}

// [[Rcpp::export]]
Rcpp::List bridge_mcmcFunctionalDataMissing(Rcpp::NumericMatrix data, Rcpp::NumericMatrix baseMeasure, double r, Rcpp::NumericVector hyperWeights, Rcpp::NumericVector iterations, double p0 = 0.5, unsigned int lag = 1, unsigned int n0 = 10) {
  // Llama a la función original para datos faltantes en mcmcFD.cpp
  return mcmcFunctionalDataMissingCpp(data, baseMeasure, r, hyperWeights, iterations, p0, lag, n0);
}

// [[Rcpp::export]]
Rcpp::NumericVector bridge_dbSilhouette(Rcpp::NumericMatrix data, Rcpp::List postPartitionMCMC) {
  // Llama a la función original dentro de dbSilhouette.cpp
  return dbSilhouetteCpp(data, postPartitionMCMC);
}
