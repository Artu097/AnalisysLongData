R <- c(2,5,10,15,20,40,60)
M <- c(10,20)


dbs.med <- array(dim=c(length(M),length(R),100))
dbs.mean <- array(dim=c(length(M),length(R),100))

for (m in M) {

  for (s in 1:100) {
      id <- sprintf("%03d",s)
      load(paste("/home/fabian/ruth/outputs/qin/output",id,"_",m,".RData",sep=""))

    for (r in 1:length(R)) {
    
#       cat(paste(id,"-",m,"-",r,"\n"))
        
      dbs.med[m/10,r,s] <- median(dbs[[paste(R[r])]]$dbs)
      dbs.mean[m/10,r,s] <- mean(dbs[[paste(R[r])]]$dbs)

    }
  }
}

dbs.raw.med <-  matrix(ncol=2,nrow=100)
dbs.raw.mean <-  matrix(ncol=2,nrow=100)

for (m in 1:length(M)) {
  for (s in 1:100) {
    id <- sprintf("%03d",s)
    load(paste("/home/fabian/ruth/data/qin/data",id,"_",M[m],".RData",sep=""))

#     cat(paste(id,"-",m,"\n"))
    
    dbs.raw.med[s,m] <- median(dbs.raw$dbs)
    dbs.raw.mean[s,m] <- mean(dbs.raw$dbs)
  }
}

save(file="~/ruth/outputs/qin/dbs.RData",list=ls())
