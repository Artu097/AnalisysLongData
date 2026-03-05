source("~/ruth/scripts/selectSample.r")


for (r in c(2,5,10,15,20,40,60)) {

load(paste("~/ruth/output/qinMis20R",r,".RData",sep=""))

# par(mfrow=c(1,2))
# plot(ergK,type="l")
# plot(mcmc$k)
# acf(mcmc$k)
#
# table(mcmc$k)

mcmc <- selectSample(mcmc,5000,2000) 

save(file=paste("~/ruth/output/qin_sampleMis20R",r,".RData",sep=""),list=c("P0","mcmc","r"))

}

