# Compilar y cargar
mi_ruta <- "C:/Users/artur/serviciosocial/paquetesR/codes"
Sys.setenv("PKG_CXXFLAGS" = paste0("-I\"", mi_ruta, "/src\""))
Rcpp::sourceCpp(file.path(mi_ruta, "main_bridge.cpp"))
source(file.path(mi_ruta, "r/mcmcFD.r"))
source(file.path(mi_ruta, "test/stats.r"))


# Cargar datos y limpieza
datos_raw <- read.csv(file.path(mi_ruta, "test/data/imc1.dat"), header=FALSE, sep=" ")
datos_mat <- matrix(as.numeric(as.matrix(datos_raw)), nrow=nrow(datos_raw))

#Analisis

res_imc1 <- bridge_mcmcFunctionalData(datos_mat, matrix(c(0, 40, 0.1, 0.1), 1, 4), 5.0, c(1,1), c(calenta,iteraciones))

#post1
post1 <- computePosteriors(res_imc1)

# Calculo de silueta
source(file.path(mi_ruta, "r/dbSilhouette.r"))
dbs1 <- dbSilhouette(datos_mat, post1[[1]])
# Guardar datos 
save(res_imc1, post1, dbs1, datos_mat, file = file.path(mi_ruta, "respaldo_analisis.RData"))
message("Información guardada")