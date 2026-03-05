source("~/ruth/codes/r/mcmcFD.r")
source("~/ruth/prev/sims/scripts/stats.r")
# source("~/ruth/scripts/selectSample.r")
source("~/ruth/codes/r/dbSilhouette.r")
# source("~/ruth/sims/scripts/plots.r")
# source("~/ruth/scripts/predictiva.r")

t0 <- Sys.time()

imc0 <- read.csv("~/ruth/data/imc0.dat",header=FALSE,sep=" ")
imc1 <- read.csv("~/ruth/data/imc1.dat",header=FALSE,sep=" ")

bb <- 5000
ss <- 5000
P0 <- c(0,40,0.1,0.1)
l0 <- c(1,1)

r <- 5

# set.seed(3098)
set.seed(3598)
mcmc0 <- mcmcFunctionalData(imc0,P0,r,l0,c(bb,ss))
out0 <- computePosteriors(mcmc0)
dbs0 <- dbSilhouette(imc0,out0[[1]])

# set.seed(3098)
set.seed(3598)
mcmc1 <- mcmcFunctionalData(imc1,P0,r,l0,c(bb,ss))
out1 <- computePosteriors(mcmc1)
dbs1 <- dbSilhouette(imc1,out1[[1]])

# save(file=paste("~/ruth/output/imcR",r,"c.RData",sep=""),list=c("mcmc0","out0","dbs0","mcmc1","out1","dbs1","r"))
save(file=paste("~/ruth/output/imcR",r,".RData",sep=""),list=c("mcmc0","out0","dbs0","mcmc1","out1","dbs1","r","P0","l0","bb","ss"))

t1 <- Sys.time()

difftime(t1,t0)

table(mcmc0$k)
table(mcmc1$k)
