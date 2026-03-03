source("~/ruth/codes/r/mcmcFD.r")
source("~/ruth/scripts/stats.r")
source("~/ruth/codes/r/dbSilhouette.r")


bb <- 5000 
ss <- 2000
P0 <- c(0,40,0.1,0.1)

cat(paste(Sys.time(),"\n"))
for (s in 26:50) {
  for (M in c(10,20)) {
  id <- sprintf("%03d",s)
  load(paste("~/ruth/data/qin/data",id,"_",M,".RData",sep=""))
  cat(paste("running",id,"\n"))
  mcmc <- list()
  post <- list()
  dbs <- list()
  for (r in c(2,5,10,15,20,40,60)) {
    cat(paste("\t r=",r,"\n"))
    set.seed(seed+1)
    mcmc0 <- mcmcFunctionalData(data,P0,r,c(1.1,1.1),c(bb,ss))
    post0 <- computePosteriors(mcmc0)
    dbs0 <- dbSilhouette(data,post0[[1]])
    mcmc[[paste(r)]] <- mcmc0
    post[[paste(r)]] <- post0
    dbs[[paste(r)]] <- dbs0
  }
  save(file=paste("~/ruth/outputs/qin/output",id,"_",M,".RData",sep=""),list=c("bb","ss","P0","mcmc","post","dbs"))
  }
}
cat(paste(Sys.time(),"\n"))

