computePosteriors <- function(mcmc) {
  mm <- nrow(mcmc$th)/sum(mcmc$Nmax)

	i0 <- 1
	th <- vector("list",length(mcmc$k))
	for (i in 1:length(mcmc$k)) {
		th[[i]] <- array(NA,c(mcmc$Nmax[i],mm,ncol(mcmc$th)))
		for (j in 1:mcmc$Nmax[i]) {
			th[[i]][j,,] <- mcmc$th[i0 + 0:(mm-1),]
			i0 <- i0 + mm
		}
	}

	postThD <- vector("list",0)
	for (i in 1:length(mcmc$k)) {
		udd <- unique(mcmc$d[i,])
		uth <- array(NA,c(mcmc$k[i],1,mm,ncol(mcmc$th)))
		for (j in 1:mcmc$k[i]) {
			uth[j,1,,] <- th[[i]][udd[j],,]
		}
		
		id0 <- 1
		new.dd= TRUE
		while(id0 <= length(postThD)) {
			if (length(postThD[[id0]]$d) == mcmc$k[i]) {
				if (all(postThD[[id0]]$d==udd)) {
					postThD[[id0]]$f <- postThD[[id0]]$f + 1
					postThD[[id0]]$th <- abind::abind(postThD[[id0]]$th,uth,along=2)
					new.dd= FALSE
					break
				}
			}
			id0 <- id0 + 1
		}
		if (new.dd) {
			postThD[[length(postThD)+1]] <- list(d=udd,f=1,p=mcmc$p[i,],o=mcmc$d[i,])
			postThD[[id0]]$th <- uth
		}
	}
	postThD <- postThD[order(as.numeric(lapply(postThD,"[[","f")),decreasing=TRUE)]

	# list(ThD=postThD,k=mcmc$k,l=ll,max=mN,N=NN,p=pp)
  postThD
}

