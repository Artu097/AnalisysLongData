#Sys.setenv("PKG_CXXFLAGS"="-I/home/fabian/Escuela/codes/src")
#Rcpp::sourceCpp("/home/fabian/Escuela/codes/r/mcmcFD.cpp",rebuild = TRUE)


mcmcFunctionalData <- function(data, baseMeasure, r, hyperWeights, iterations, p0=0.5, lag=1, n0=10) {
  dd <- data
  if (!is.matrix(data)) {
    dd <- as.matrix(data)
  }

  P0 <- baseMeasure
  if (!is.matrix(P0)) {
    P0 <- t(as.matrix(P0))
  }

  if (any(is.na(data)) || any(apply(data,2,is.nan))) {
    bridge_mcmcFunctionalDataMissing(dd, P0, r, hyperWeights, iterations, p0, lag, n0)
  } else {
    bridge_mcmcFunctionalData(dd, P0, r, hyperWeights, iterations, p0, lag, n0)
  }
}
