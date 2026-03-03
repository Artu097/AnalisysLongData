source("~/ruth/sims/scripts/stats.r")
source("~/ruth/codes/r/dbSilhouette.r")


dd <- read.csv("~/ruth/data/simQinMis20.dat",header=FALSE,sep=" ")

for (r in c(2,5,10,15,20,40,60)) {
  load(paste("~/ruth/output/qin_sampleMis20R",r,".RData",sep=""))
  post <- computePosteriors(mcmc)
  # dbs <- dbSilhouette(dd,post[[1]])
  save(file=paste("~/ruth/output/qin_posteriorMis20R",r,".RData",sep=""),list=c("post","dbs","r"))
}


