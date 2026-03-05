# 1. Definir ruta y limpiar entorno
mi_ruta <- "C:/Users/artur/serviciosocial/paquetesR/codes"
setwd(mi_ruta)

# 2. Configurar compilador y cargar el Bridge
Sys.setenv("PKG_CXXFLAGS" = paste0("-I\"", mi_ruta, "/src\""))
Rcpp::sourceCpp("main_bridge.cpp", rebuild = TRUE)

# 3. Cargar funciones (asegúrate de que los archivos estén limpios)
source("r/mcmcFD.r")
source("r/dbSilhouette.r")
source("test/stats.r")

# 4. Cargar y preparar datos de imc1.dat
datos_raw <- read.csv("test/data/imc1.dat", header=FALSE, sep=" ")
datos_mat <- as.matrix(datos_raw)
# Forzamos a que sea numérico para evitar NaNs
datos_mat <- matrix(as.numeric(datos_mat), nrow=nrow(datos_mat))

# 5. Configurar parámetros del modelo
# m0, c0, a0, b0
P0_mat <- matrix(c(0, 40, 0.1, 0.1), nrow=1, ncol=4) 
r_val  <- 5.0
pesos  <- c(1.0, 1.0)
iter   <- c(1000, 1000) # Burn-in y Muestreo

# 6. Ejecución
message("Ejecutando MCMC para imc1.dat...")
res_imc1 <- mcmcFunctionalData(datos_mat, P0_mat, r_val, pesos, iter)

# 7. Verificar resultados
print("Número de grupos detectados en la última iteración:")
print(res_imc1$k[length(res_imc1$k)])