/*! @file random.hpp
 *  Class used to simulate random numbers
 */
#ifndef my_RANDOM_HPP
#define my_RANDOM_HPP

/*! @def R_RANDOM_NUMBERS
 *  If it is selected, the class random will use R functions to generate random numbers
 *  @todo Finish this implementation, search in documentation and internet
 *
 *  @def GSL_RANDOM_NUMBERS
 *  Another option for the random number functions is using GSL
 */

// #include <cmath>

/*
template <class T>
T max(const T &a, const T &b) {
	return (a>b)?a:b;
}

int ceiling(real a) {
	if (a!=real(int(a))) return int(a)+(a>=0.0)?1:0;
	return int(a);
}
*/

#ifdef USE_R_RGN
	#include "random_r.hpp"
	typedef math::random_r _r;
#else
	#include "random_gsl.hpp"
	typedef math::random_gsl _r;
#endif

#endif // my_RANDOM_HPP
