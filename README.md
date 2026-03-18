AnalisysLongData es un paquete de R diseñado para el agrupamiento (clustering) de datos funcionales longitudinales, 
con un enfoque específico en trayectorias de Índice de Masa Corporal (IMC). 
Utiliza un backend en C++ vía Rcpp para realizar inferencia Bayesiana mediante cadenas de Markov (MCMC).
Este proyecto fue desarrollado como parte del servicio social de Arturo González Cruz en la Universidad Autónoma Metropolitana unidad Iztapalapa. Instalación:
Es necesario que cuentes con Rtools, que puedes descargarlo desde: 
https://cran.r-project.org/bin/windows/Rtools/

Además de icontar con devtools:
R# Si no tienes devtools instalado:

install.packages("devtools")

devtools::install_github("Artu097/AnalisysLongData")
Uso BásicoPara realizar una simulación o análisis de trayectorias.
Solo necesitas una matriz de datos donde cada fila represente a un individuo y cada columna un punto en el tiempo.Rlibrary(AnalisysLongData)

# 1. Cargar los datos de ejemplo incluidos en el paquete
ruta_ejemplo <- system.file("extdata", "imc1.dat", package = "AnalisysLongData")

# Leer y convertir a matriz
datos_raw <- read.table(ruta_ejemplo, header = FALSE, sep = " ")

datos_mat <- as.matrix(datos_raw)

datos_mat <- matrix(as.numeric(datos_mat), nrow = nrow(datos_mat))

# 2. Definir parámetros de la Medida Base (m0, c0, a0, b0)
P0 <- matrix(c(0, 40, 0.1, 0.1), nrow = 1, ncol = 4)

# 3. Correr el modelo MCMC
resultado <- mcmcFunctionalData(

    data         = datos_mat, 
    
    baseMeasure  = P0, 
    
    r            = 5.0, 
    
    hyperWeights = c(1, 1), 
    
    iterations   = c(1000, 1000) # Burn-in y Muestreo
    
)

# 4. Generar reportes visuales (PDFs por grupo)
pp_final <- resultado$p[nrow(resultado$p), ]

generar_graficas_grupos(datos_mat, resultado$th, pp_final, getwd())

Parámetros de SimulaciónPara que otro usuario realice sus propias simulaciones, estos son los valores que puede ajustar:
ParámetroDescripciónValor sugeridodataMatriz numérica de trayectorias (sin NAs preferentemente).
Datos de IMC longitudinal.baseMeasureParámetros $P_0$ para la distribución base ($m_0, c_0, a_0, b_0$).matrix(c(0, 40, 0.1, 0.1), 1, 4)r
Parámetro de concentración del proceso de Dirichlet.Mayor $r = $ más grupos detectados.iterationsVector con c(BurnIn, Sampling).c(1000, 1000) para estabilidad.
