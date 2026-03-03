#Sys.setenv("PKG_CXXFLAGS"="-I/home/fabian/Escuela/codes/src")
#Rcpp::sourceCpp("/home/fabian/Escuela/codes/r/dbSilhouette.cpp",rebuild = TRUE)

dbSilhouette <- function(data,mcmcStats) {
  dd <- data
  if (!is.matrix(data)) {
    dd <- as.matrix(data)
  }
  dbs <- c()
  dbs <- bridge_dbSilhouette(dd, mcmcStats)
  pp <- mcmcStats$p
  idxP <- seq_len(length(pp))
  oo <- unlist(sapply(unique(pp),function(j) { (idxP[pp==j])[order(dbs[pp==j],decreasing=TRUE)] }))
  list(dbs.y=dbs, part=pp, order=oo, dbs=dbs[oo])
}

