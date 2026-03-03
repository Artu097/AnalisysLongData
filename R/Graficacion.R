# Cuidar las dimensiones
M <- ncol(datos_mat) # Debe ser 8
eje_x <- 1:M

for (t in 1:dim(th1)[1]) {
  pdf(file.path(mi_ruta, paste0("test/imgs/imc1_res/grupo_imc1_", t, ".pdf")), width=4, height=3)
  par(mar=c(3,3,1,1), bty="L", cex=0.6, family="serif")
  
  plot(NULL, xlim=c(1, M), ylim=range(datos_mat, na.rm=TRUE), 
       xlab="Tiempo", ylab="IMC", main=paste("Grupo", t))
  
  #Trayectorias individuales en gris
  for (i in seq_len(nrow(datos_mat))) {
    if (pp1[i] == t) {
      lines(eje_x, datos_mat[i, ], col="gray80", lwd=0.5)
    }
  }
  
  #Calculo de la media
  # Estructura: th1[grupo, iteracion, parametro, tiempo]
  #media_final <- apply(th1[t, , 1, ], 2, mean)
  media_final <- colMeans(th1[t, , ,1 ])
  
  lines(eje_x, media_final[1:M], col="red", lwd=3)
  
  dev.off()
}

message("Gráficas guardadas en")