source("~/ruth/prev/sims/scripts/plots.r")
source("~/ruth/scripts/tmp/predictiva.r")

r <- 5
M <- ncol(imc1)

# load(paste("~/ruth/output/imcR",r,".RData",sep=""))

th0 <- out1[[1]]$th
pp0 <- out1[[1]]$p

par(mfrow=c(3,3))
for (t in 1:dim(th0)[1]) {
	set.seed(9876)
	xm1 <- intPostD(2,th0,t)
# 	pdf(paste("/home/fabian/ruth/imgs/kl/group",t,"i1.pdf",sep=""),80/25.4,40/25.4)
		par(mar=c(3,3,0,0)+0.2,bty="L",cex=0.5,family="serif")
		plot(NULL,xlim=c(1,M),ylim=range(imc1),xlab="",ylab="") #,main=paste("G",t))
		plotIntervals(xm1$int,"cyan")
    for (i in seq_len(length(pp0))) {
      if (pp0[i]==t) {
        lines(1:ncol(imc1),imc1[i,])
      }
    }
		lines(xm1$mean,col="red",lwd=4)
# 	dev.off()
}

# source("~/ruth/codes/r/dbSilhouette.r")
source("~/ruth/scripts/plots.r")

X11()
par(mfrow=c(1,2))
pdf("~/ruth/imgs/imc/dbs0R5.pdf",6,5,family="Times")
par(mar=c(4,0,0,0)+0.1,family="serif")
plot.dbs(dbs0)#,xlab=expression(italic("dbs")))
dev.off()

pdf("~/ruth/imgs/imc/dbs1R5.pdf",6,5,family="Times")
par(mar=c(4,0,0,0)+0.1,family="serif")
plot.dbs(dbs1)#,xlab=expression(italic("dbs")))
dev.off()

