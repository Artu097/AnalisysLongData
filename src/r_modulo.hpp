// C:\Users\artur\Servicio Social\paquetesR\codes\r

#ifndef R_MODULO_HPP
#define R_MODULO_HPP

// Subimos un nivel para salir de 'r' y entramos a 'src'
#include "../src/src_modulo.hpp"

#include "dbSilhouette.hpp"
#include "dbSilhouetteQuotient.hpp"

// Incluimos la lógica para que Rcpp la encuentre
#include "dbSilhouette.cpp"
#include "mcmcFD.cpp"

#endif