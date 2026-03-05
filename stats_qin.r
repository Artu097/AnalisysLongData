source("~/ruth/sims/scripts/plots.r")
source("~/ruth/scripts/predictiva.r")

dd <- read.csv("~/ruth/data/simQin.dat",header=FALSE,sep=" ")

M <- ncol(dd)

for (r in c(2,5,10,15,20,40,60)) {

load(paste("~/ruth/output/qin_posteriorMis20R",r,".RData",sep=""))

dd <- mm

th0 <- post[[1]]$th
pp0 <- post[[1]]$p

# par(mfrow=c(2,3))
for (t in 1:dim(th0)[1]) {
	set.seed(9876)
	xm1 <- intPostD(2,th0,t)
	pdf(paste("/home/fabian/ruth/imgs/qin/mis/R",r,"/media",t,".pdf",sep=""),80/25.4,40/25.4)
		par(mar=c(3,3,0,0)+0.2,bty="L",cex=0.5,family="serif")
		plot(NULL,xlim=c(1,M),ylim=range(dd),xlab="",ylab="")
		plotIntervals(xm1$int,"cyan")
    for (i in seq_len(length(pp0))) {
      if (pp0[i]==t) {
        lines(1:ncol(dd),dd[i,])
      }
    }
		lines(xm1$mean,col="red",lwd=4)
	dev.off()
}

}

######

# X11()
# plot.dbs(dbs)

