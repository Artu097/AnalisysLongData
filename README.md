*AnalisysLongData* es un paquete de R diseñado para el agrupamiento (clustering) de datos funcionales longitudinales, con un enfoque específico en trayectorias del Índice de Masa Corporal (IMC).

El paquete utiliza un backend en C++ mediante Rcpp para realizar inferencia Bayesiana a través de métodos de cadenas de Markov Monte Carlo (MCMC).

Universidad Autónoma Metropolitana Unidad Iztapalapa.

# 1 Instalación

Antes de instalar el paquete, asegúrate de contar con:

- Rtools (para compilar código en Windows):
https://cran.r-project.org/bin/windows/Rtools/

- devtools en R:

```
# Instalar devtools (si no lo tienes)
install.packages("devtools")

# Instalar el paquete desde GitHub
devtools::install_github("Artu097/AnalisysLongData")
```
# Uso básico

Para realizar una simulación o analizar trayectorias, necesitas una matriz donde:

- Cada fila representa un individuo

- Cada columna representa un punto en el tiempo

1. Cargar la librería
```
library(AnalisysLongData)
```
2. Cargar datos de ejemplo
```
ruta_ejemplo <- system.file("extdata", "imc1.dat", package = "AnalisysLongData")
datos_raw <- read.table(ruta_ejemplo, header = FALSE, sep = " ")
datos_mat <- as.matrix(datos_raw)
datos_mat <- matrix(as.numeric(datos_mat), nrow = nrow(datos_mat))
```
3. Definir parámetros
```
P0 <- matrix(c(0, 40, 0.1, 0.1), nrow = 1, ncol = 4)
```
4. Ejecutar el modelo MCMC
```
resultado <- mcmcFunctionalData(
  data         = datos_mat,
  baseMeasure  = P0,
  r            = 5.0,
  hyperWeights = c(1, 1),
  iterations   = c(1000, 1000) # Burn-in y muestreo
)
```
5. Generar las gráficas de los resultados en formato PDF
```
pp_final <- resultado$p[nrow(resultado$p), ]

generar_graficas_grupos(
  datos_mat,
  resultado$th,
  pp_final,
  getwd()
)
```
# Salida 
El paquete genera:

- Resultados del modelo MCMC

- Asignaciones de clusters

- Gráficas en PDF por grupo
# Detalles
- Asegúrate de que los datos estén correctamente formateados como matriz numérica.
- El número de iteraciones puede ajustarse según la complejidad del análisis.

# Autores

**Asael Fabián Martínez Martínez**  
Autor principal. Desarrollo del modelo, metodología y código fuente original.

**Arturo González Cruz**  
Desarrollo del paquete en R, integración con C++ mediante Rcpp, generación de visualizaciones y mantenimiento del repositorio en GitHub.
