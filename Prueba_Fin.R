# 1. Definir rutas reales (Ajusta si cambiaste de carpeta)
mi_ruta <- "C:/Users/artur/serviciosocial/paquetesR/codes"
setwd(mi_ruta)

# 2. Configurar el compilador e integrar el Bridge
# Asegúrate de que main_bridge.cpp no tenga etiquetas dentro
Sys.setenv("PKG_CXXFLAGS" = paste0("-I\"", mi_ruta, "/src\""))
Rcpp::sourceCpp("main_bridge.cpp", rebuild = TRUE)

# 3. Cargar envoltorios de R (Limpia los archivos .r de rutas viejas antes)
source("r/mcmcFD.r")
source("r/dbSilhouette.r")
source("test/stats.r")

# 4. Cargar y Limpiar Datos de IMC
# Es vital asegurar que todos los datos sean numéricos
datos_raw <- read.csv("test/data/imc0.dat", header=FALSE, sep=" ")
datos_mat <- as.matrix(datos_raw)
datos_mat <- matrix(as.numeric(datos_mat), nrow=nrow(datos_mat)) # Forzar numérico

# 5. Configurar Parámetros (Asegúrate de que sean simples y sin NAs)
P0_vec <- c(0, 40, 0.1, 0.1) # m0, c0, a0, b0
r_val  <- 5.0
pesos  <- c(1.0, 1.0)
iter   <- c(500, 500) # Prueba rápida: 500 burn-in, 500 muestras

# 6. Ejecución con validación previa
if (!any(is.na(datos_mat)) && is.numeric(r_val)) {
  message("Iniciando MCMC...")
  res <- mcmcFunctionalData(datos_mat, P0_vec, r_val, pesos, iter)
  
  # 7. Procesar Resultados
  post <- computePosteriors(res)
  message("¡Éxito! Grupos encontrados en la última iteración: ", res$k[length(res$k)])
} else {
  stop("Error: Los datos contienen NAs o los parámetros no son numéricos.")
}