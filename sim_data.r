source("C:/Users/artur/serviciosocial/paquetesR/codes/r/dbSilhouette.r")

curvedata <- function(k,j) { k*sin(pi*j*k/10 - pi*k/4)/2 }
linedata <- function(j,s=sample(0:1,1)) { -1 + (-1)^(s)*j/10 }


M <- 10
Nj <- 30

S2 <- 0.3

realX <- list()
realX$d <- 1:5
realX$f <- 1
realX$p <- rep(1:5,each=Nj)
realX$o <- realX$p
thT <- vector("list",1)
thT[[1]] <- array(NA,c(5,M,2))
for (j in 1:3) {
thT[[1]][j,,1] <- curvedata(j,1:M)
thT[[1]][j,,2] <- rep(1/S2,M)
}
thT[[1]][4,,1] <- linedata(1:M,0)
thT[[1]][4,,2] <- rep(1/S2,M)
thT[[1]][5,,1] <- rep(0,M)
thT[[1]][5,,2] <- rep(3/(1+3*S2),M)
realX$th <- abind::abind(thT,along=2)

save(file=paste("~/ruth/data/qin/real_",M,".RData",sep=""),list=c("realX","S2"))

set.seed(30919)

ss <- round(runif(100,1e4,1e6),0)

for (s in seq_len(length(ss))) {
  seed <- ss[s]
  set.seed(seed)
  data <- matrix(0,5*Nj,M)

  for (i in 1:Nj) {
    data[i,] <- curvedata(1,1:M) + rnorm(M,0,sqrt(S2))
    data[Nj+i,] <- curvedata(2,1:M) + rnorm(M,0,sqrt(S2))
    data[2*Nj+i,] <- curvedata(3,1:M) + rnorm(M,0,sqrt(S2))
    data[3*Nj+i,] <- linedata(1:M,0) + rnorm(M,0,sqrt(S2))
    data[4*Nj+i,] <- runif(M,-1,1) + rnorm(M,0,sqrt(S2))
  }
  dbs.raw <- dbSilhouette(data,realX)

  id <- sprintf("%03d",s)
  save(file=paste("C:/Users/artur/serviciosocial/paquetesR/codes/test/data/",id,"_",M,".RData",sep=""), list=c("data","seed","dbs.raw"))
}
