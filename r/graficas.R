#' Generar reportes de trayectorias por grupo
#' @export
generar_graficas_grupos <- function(datos_mat, th1, pp1, mi_ruta) {

  M <- ncol(datos_mat)
  eje_x <- 1:M

  # Determinamos cuántos grupos hay realmente (ej. 10)
  n_grupos <- max(pp1, na.rm = TRUE)

  # Crear carpeta si no existe
  dir_imgs <- file.path(mi_ruta, "test/imgs/imc1_res")
  dir.create(dir_imgs, recursive = TRUE, showWarnings = FALSE)

  message("Generando ", n_grupos, " reportes en PDF...")

  for (t in 1:n_grupos) {
    pdf(file.path(dir_imgs, paste0("grupo_imc1_", t, ".pdf")), width=4, height=3)
    par(mar=c(3,3,1,1), bty="L", cex=0.6, family="serif")

    plot(NULL, xlim=c(1, M), ylim=range(datos_mat, na.rm = TRUE),
         xlab="Tiempo", ylab="IMC", main=paste("Grupo", t))

    # 1. Dibujar trayectorias individuales en gris
    indices_grupo <- which(pp1 == t)
    for (i in indices_grupo) {
      lines(eje_x, datos_mat[i, ], col="gray80", lwd=0.5)
    }

    # 2. Dibujar la media del grupo en rojo (Calculada de los datos)
    if(length(indices_grupo) > 0) {
      if(length(indices_grupo) > 1) {
        media_final <- colMeans(datos_mat[indices_grupo, , drop = FALSE], na.rm = TRUE)
      } else {
        media_final <- datos_mat[indices_grupo, ]
      }
      lines(eje_x, media_final, col="red", lwd=2)
    }

    dev.off()
  }

  message("¡Listo! Gráficas guardadas en: ", dir_imgs)
}
