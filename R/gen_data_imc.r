imc <- read.csv("~/ruth/data/imc0.dat",header=FALSE,sep=" ")

n <- nrow(imc)
s <- round(0.7*n)

set.seed(98104)
ii <- sample.int(n,s)
imc.s <- imc[ii,]

write(t(imc.s),"~/ruth/data/imc0sample.dat",ncol(imc.s))

imc <- read.csv("~/ruth/data/imc1.dat",header=FALSE,sep=" ")

n <- nrow(imc)
s <- round(0.7*n)

set.seed(98104)
ii <- sample.int(n,s)
imc.s <- imc[ii,]

write(t(imc.s),"~/ruth/data/imc1sample.dat",ncol(imc.s))
