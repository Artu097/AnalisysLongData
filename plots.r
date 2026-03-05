mi_ruta <- "C:/Users/artur/serviciosocial/paquetesR/codes"

source(file.path(mi_ruta, "test/sim_study/predictiva.r")) 
source(file.path(mi_ruta, "test/sim_study/plots.r"))      

s <- 40
m <- 20
id <- sprintf("%03d",s)
load(paste("/home/fabian/ruth/outputs/qin/output",id,"_",m,"_c80.RData",sep=""))

load(paste("~/ruth/data/qin/data",id,"_",m,".RData",sep=""))

# th0 <- out0[[1]]$th
r <- 15
pp0 <- post[[paste(r)]][[1]]

X11()
par(mfrow=c(2,3))
uu <- unique(pp0$d)
for (u in uu) {
  plot(NULL,xlim=c(1,m),ylim=range(data))
  for (i in 1:nrow(data)) {
    if (pp0$o[i]==u) lines(data[i,],col=u+2)
  }
}

X11()
plot.dbs(dbs[[paste(r)]])
