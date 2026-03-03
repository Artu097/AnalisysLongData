R <- c(2,5,10,15,20,40,60)
M <- c(10,20)


groups <- array(dim=c(2,length(M),length(R),100))

for (m in M) {
  for (r in 1:length(R)) {
    for (s in 1:100) {
      id <- sprintf("%03d",s)
      load(paste("/home/fabian/ruth/outputs/qin/output",id,"_",m,".RData",sep=""))

      groups[1,m/10,r,s] <- length(post[[paste(R[r])]][[1]]$d)
      prk <- table(mcmc[[paste(R[r])]]$k)
      groups[2,m/10,r,s] <- as.numeric(names(prk)[which.max(prk)])
    }
  }
}

save(file="~/ruth/outputs/qin/groups.RData",list=ls())
