/*! @file random_gsl.hpp
 *  Random number generator based on GSL
 */

#ifndef my_RANDOM_GSL_HPP
#define my_RANDOM_GSL_HPP

#include "datatypes.h"
#include "vector.hpp"

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_sf_exp.h>
#include <gsl/gsl_sf_log.h>

#include <cmath>

#include <iostream>

namespace math {

///-----------------------------------------------------------------------------
/// Random class; declaration
///-----------------------------------------------------------------------------

/*! @class random_gsl
 *  This class is a wrap to simulate random numbers.
 *
 *  Simulation algorithms will be taken from among the different available libraries, e.g. gsl or R.
 */
class random_gsl {
private:
    /// According to the selected random number generator library, some variables could be required
//#ifdef GSL_RANDOM_NUMBERS
    static gsl_rng *randGen;
//#endif

public:

	~random_gsl();

    //! @brief Initializes the random generator

//#ifdef GSL_RANDOM_NUMBERS
    static void setup(unsigned long int seed, const gsl_rng_type *method);
//#endif

//#ifdef R_RANDOM_NUMBERS
//    static void setup(unsigned int seed1, unsigned int seed2);
//#endif

    //! @brief Frees the random generator
    static void free();

    static void readConsole(std::ostream &os, std::istream &is);
    static void show(std::ostream &os);

    /// Random number generators and probability density functions (pdf)

    //! @brief Uniform distributions
    //! @param a [in] lower limit
    //! @param b [in] upper limit
    //! @param p [in] parameter vector (a,b)

    //! @brief Continuous uniform distribution
    //! @return (0,1)-uniform distributed random number
    static real rndUniform();

    //! @return (a,b)-uniform distributed random number
    static real rndUniform(const real a, const real b);
    static real rndUniform(const _vector<real> &p);

    //! @brief Discrete uniform distribution
    //! @return [a,b]-uniform distributed random number
    static sint rndDUniform(sint a, sint b);
    static sint rndDUniform(const _vector<sint> &p);

    //! @brief Discrete uniform distribution
    //! @return [0,a]-uniform distributed random number
    static unsigned int rndDUniform(unsigned int a);

    //! @brief Normal distribution
    //! @param m [in] media
    //! @param s [in] variance
    //! @param p [in] parameter vector (m,s)
    /*! Probability density function given by
        @f[
            f(x;m,s)=\frac1{\sqrt{2\pi s}}\exp\left\{-\frac12\frac{(x-m)^2}{s^2}\right\}
        @f]
     */

    //! @brief Generator
    //! @return normal distributed random number
    static real rndNormal(const real m, const real v);
    static real rndNormal(const _vector<real> &p);

    //! @brief Density function
    //! @return pdf for the normal distribution
    static real pdfNormal(const real x, const real m, const real v);
    static real pdfNormal(const real x, const _vector<real> &p);


    //! @brief Gamma distribution
    //! @param a [in] shape
    //! @param b [in] scale
    //! @param p [in] parameter vector (a,b)
    /*! Probability density function given by
        @f[
            f(x;a,b)=\frac{b^a}{\Gamma(a)}x^{a-1}e^{-bx}
        @f]
     */

    //! @brief Generator
    //! @return gamma distributed random number
    static real rndGamma(const real a, const real b);
    static real rndGamma(const _vector<real> &p);

    //! @brief Density function
    //! @return pdf for the gamma distribution
    static real pdfGamma(const real x, const real a, const real b);
    static real pdfGamma(const real x, const _vector<real> &p);

/*    //! @brief Normal-Gamma distribution
    //! @param m [in] media
    //! @param t [in] variance
    //! @param a [in] shape
    //! @param b [in] scale
    //! @param p [in] parameter vector (m,t,a,b)
    / *! Probability density function given by
            @f{align*}{
                f(x,y;m,c,a,b)&=pdf_N(x;m,y/t)pdf_G(y;a,b)\\
                        &\frac{t^{1/2}}{\sqrt{2\pi y}}\exp\left\{-\frac12\frac{t(x-m)^2}{y^2}\frac{b^a}{\Gamma(a)}y^{a-1}e^{-by}\right\}
            @f}
         */

    //! @brief Generator
    //! @return normal-gamma distributed random number
    static _vector<real> rndNormalGamma(const real m, const real c, const real a, const real b);
    static _vector<real> rndNormalGamma(const _vector<real> &p);

    //! @brief Density function
    //! @return pdf for the gamma distribution
    static real pdfNormalGamma(const _vector<real> &x, const real m, const real c, const real a, const real b);
    static real pdfNormalGamma(const _vector<real> &x, const _vector<real> &p);

    //! @brief Beta distribution
    //! @param a [in] shape 1
    //! @param b [in] shape 2
    //! @param p [in] parameter vector (a,b)
    /*! Probability density function given by
        @f{align*}{
            f(x;a,b)= \frac{\Gamma(a+b)}{\Gamma(a)\Gamma(b)}x^{a-1}(1-x)^{b-1}
        @f}
     */

    //! @brief Generator
    //! @return beta distributed random number
    static real rndBeta(const real a, const real b);
    static real rndBeta(const _vector<real> &p);

    //! @brief Density function
    //! @return pdf for the beta distribution
    static real pdfBeta(const real x, const real a, const real b);
    static real pdfBeta(const real x, const _vector<real> &p);

    //! @brief Geometric distribution
    //! @param p [in] probability
    /*! Probability density function given by
        @f[
            f(x;p)= p(1-p)^x; x= 0, 1, 2, ...
        @f]
     */

    //! @brief Generator
    //! @return Geometric distributed random number
    static unsigned int rndGeometric(const real p);

    //! @brief Density function
    //! @return pdf for the Geometric distribution
    static real pdfGeometric(unsigned int x, const real p);

    //! @brief Exponential distribution
    //! @param lambda [in] parameter
    //! @param U [in] truncated at U, x<=U
    /*! Probability density distribution given by
        @f[
	    f(x;\lambda,U)= \frac{\lambda e^{-\lambda x}}{1-e^{-\lambda U}}
        @f]
     */
    //! @brief second version
    //! @param L [in] truncated at L, x>=L
    /*! Probability density distribution given by
        @f[
	    f(x;\lambda,L)= \frac{\lambda e^{-\lambda x}}{e^{-\lambda L}}
        @f]
     */
	//! @brief third version
    //! @param L [in] truncated at L, x>=L
    //! @param U [in] truncated at U, x<=U, i.e. L<=x<=U
    /*! Probability density distribution given by
        @f[
	    f(x;\lambda,L,U)= \frac{\lambda e^{-\lambda x}}{e^{-\lambda L} - e^{-\lambda U}}
        @f]
     */

    //! @brief Generator
    //! @return Exponential distributed random number
    static real rndExponential(const real lambda);

    //! @brief Density function
    //! @return pdf for the Exponential distribution
    static real pdfExponential(const real x, const real lambda);

    //! @brief Generator
    //! @return L-truncated Exponential distributed random number
    static real rndExponentialL(const real lambda, const real L);

    //! @brief Density function
    //! @return pdf for the L-truncated Exponential distribution
    static real pdfExponentialL(const real x, const real lambda, const real L);

    //! @brief Generator
    //! @return U-truncated Exponential distributed random number
    static real rndExponentialU(const real lambda, const real U);

    //! @brief Density function
    //! @return pdf for the U-truncated Exponential distribution
    static real pdfExponentialU(const real x, const real lambda, const real U);

	//! @brief Generator
    //! @return (L,U)-truncated Exponential distributed random number
    static real rndExponentialLU(const real lambda, const real L, const real U);

    //! @brief Density function
    //! @return pdf for the (L,U)-truncated Exponential distribution
    static real pdfExponentialLU(const real x, const real lambda, const real L, const real U);


	static unsigned int rndPoisson(const real l);


    //! @brief Bernoulli random number generator
    //! @param p [in] Probability of success
    static unsigned int rndBernoulli(const real p);

    //! @brief Random index
    /*! Given a vector of weights of size n, a number from the range [0,n-1] is selected according to these probabilities.
     *  Is not required that the weights sum one.
     *  This function could be thinked as a generic discrete random number generator.
     */
    //! @param w [in] vector of weights
    //! @return Random index generator
    static unsigned int rndIndex(const _vector<real> &w, bool normalized=false);

    //! @brief For MH acceptance
    /*! This function accepts or not a proposal for
     *  a Metropolis-Hasting MCMC method
     */
    //! @param ratio [in] Ratio in the acceptance probability given by min(ratio,1)
    static bool acceptedMH(const real ratio);

}; // END random_gsl class

} // END namespace math

/* *********************************************************************** */
/* *********************************************************************** */

math::random_gsl::~random_gsl() { free(); }

//! Initializes the random generator pointer
gsl_rng *math::random_gsl::randGen=0;

//! Initializes the random generator
void math::random_gsl::setup(unsigned long int seed, const gsl_rng_type *method) {
    if (randGen==0) {
        gsl_rng_default_seed= seed;
        randGen= gsl_rng_alloc(method);
        gsl_rng_set(randGen,seed);
    }
}

//! Frees the random generator
void math::random_gsl::free() {
    if (randGen!=0) {
        gsl_rng_free(randGen);
        randGen=0;
    }
}

//! Initializes the random generator from istream
void math::random_gsl::readConsole(std::ostream &os, std::istream &is) {
    unsigned long int seed;
    os<<"Random number generator\nseed: ";
    is>>seed;
	os<<"SEED: "<<seed<<"\n";
    setup(seed,gsl_rng_mt19937);
}

void math::random_gsl::show(std::ostream &os) {
    os<<"Random number generator type: "<<gsl_rng_name(randGen)<<"\n\tseed: "<<gsl_rng_default_seed<<"\n";
}

/// Random number generators and probability density functions (pdf)

//! Uniform distributions

//! Continuous uniform distribution
//! (0,1)-uniform distributed random number
real math::random_gsl::rndUniform() {
    return gsl_rng_uniform_pos(randGen);
}

//! (a,b)-uniform distributed random number
real math::random_gsl::rndUniform(const real a, const real b) {
    return a + rndUniform()*(b-a);
}

real math::random_gsl::rndUniform(const _vector<real> &p) {
    return rndUniform(p[0],p[1]);
}

//! Discrete uniform distribution
//! [a,b]-uniform distributed random number
sint math::random_gsl::rndDUniform(sint a, sint b) {
	if (a==b) return a;
    return a + gsl_rng_uniform_int(randGen,b-a+1);
}

sint math::random_gsl::rndDUniform(const _vector<sint> &p) {
    return rndUniform(p[0],p[1]);
}

//! Discrete uniform distribution
//! [0,a]-uniform distributed random number
unsigned int math::random_gsl::rndDUniform(unsigned int a) {
    return gsl_rng_uniform_int(randGen,a+1);
}

//! Normal distribution

//! Generator
//! normal distributed random number
real math::random_gsl::rndNormal(const real m, const real v) {
    return m + gsl_ran_gaussian(randGen,sqrt(v));
}

real math::random_gsl::rndNormal(const _vector<real> &p) {
    return rndNormal(p[0],p[1]);
}

//! Density function
//! pdf for the normal distribution
real math::random_gsl::pdfNormal(const real x, const real m, const real v) {
    return gsl_ran_gaussian_pdf(x-m,sqrt(v));
}

real math::random_gsl::pdfNormal(const real x, const _vector<real> &p) {
    return pdfNormal(x,p[0],p[1]);
}


//! Gamma distribution

//! Generator
//! gamma distributed random number
real math::random_gsl::rndGamma(const real a, const real b) {
    return gsl_ran_gamma(randGen,a,1.0/b);
}

real math::random_gsl::rndGamma(const _vector<real> &p) {
    return rndGamma(p[0],p[1]);
}

//! Density function
//! pdf for the gamma distribution
real math::random_gsl::pdfGamma(const real x, const real a, const real b) {
    return gsl_ran_gamma_pdf(x,a,1.0/b);
}

real math::random_gsl::pdfGamma(const real x, const _vector<real> &p) {
    return pdfGamma(x,p[0],p[1]);
}

//! Normal-Gamma distribution

//! Generator
//! normal-gamma distributed random number
_vector<real> math::random_gsl::rndNormalGamma(const real m, const real c, const real a, const real b) {
    _vector<real> r(2);
    r[1]= rndGamma(a,b);
    r[0]= rndNormal(m,c/r[1]);
    return r;
}

_vector<real> math::random_gsl::rndNormalGamma(const _vector<real> &p) {
    return rndNormalGamma(p[0],p[1],p[2],p[3]);
}

//! Density function
//! pdf for the gamma distribution
real math::random_gsl::pdfNormalGamma(const _vector<real> &x, const real m, const real c, const real a, const real b) {
    return pdfNormal(x[0],m,c/x[1])*pdfGamma(x[1],a,b);
}

real math::random_gsl::pdfNormalGamma(const _vector<real> &x, const _vector<real> &p) {
    return pdfNormalGamma(x,p[0],p[1],p[2],p[3]);
}

//! Beta distribution

//! Generator
//! beta distributed random number
real math::random_gsl::rndBeta(const real a, const real b) {
    return gsl_ran_beta(randGen,a,b);
}

real math::random_gsl::rndBeta(const _vector<real> &p) {
    return rndBeta(p[0],p[1]);
}

//! Density function
//! pdf for the beta distribution
real math::random_gsl::pdfBeta(const real x, const real a, const real b) {
    return gsl_ran_beta_pdf(x,a,b);
}

real math::random_gsl::pdfBeta(const real x, const _vector<real> &p) {
    return pdfBeta(x,p[0],p[1]);
}

//! Geometric distribution

//! Generator
//! Geometric distributed random number
unsigned int math::random_gsl::rndGeometric(const real p) {
	// gsl_**_geometric starts at one
    return gsl_ran_geometric(randGen,p) - 1;
// // 	return ceil(gsl_sf_log(rndUniform())/gsl_sf_log(1.0-p)) - 1;
}

//! Density function
//! pdf for the Geometric distribution
real math::random_gsl::pdfGeometric(unsigned int x, const real p) {
//    return p*gsl_pow_unsigned int(1.0-p,x-M);
    return gsl_ran_geometric_pdf(x+1,p);
}

//! Exponential distribution

//! Generator
//! Exponential distributed random number
real math::random_gsl::rndExponential(const real lambda) {
    return gsl_ran_exponential(randGen,1.0/lambda);
}

//! Density function
//! pdf for the Exponential distribution
real math::random_gsl::pdfExponential(const real x, const real lambda) {
    return gsl_ran_exponential_pdf(x,1.0/lambda);
}

//! Generator
//! L-truncated Exponential distributed random number
real math::random_gsl::rndExponentialL(const real lambda, const real L) {
    return L - log(rndUniform())/lambda;
}

//! Density function
//! pdf for the L-truncated Exponential distribution
real math::random_gsl::pdfExponentialL(const real x, const real lambda, const real L) {
	return lambda*exp(-lambda*(x-L));
}

//! Generator
//! U-truncated Exponential distributed random number
real math::random_gsl::rndExponentialU(const real lambda, const real U) {
	return -log(1.0-rndUniform()*(1.0-exp(-lambda*U)))/lambda;
}

//! Density function
//! pdf for the U-truncated Exponential distribution
real math::random_gsl::pdfExponentialU(const real x, const real lambda, const real U) {
    return lambda*exp(-lambda*x)/(1.0-exp(-lambda*U));
}

//! Generator
//! (L,U)-truncated Exponential distributed random number
real math::random_gsl::rndExponentialLU(const real lambda, const real L, const real U) {
	return - log(rndUniform()*(exp(-lambda*U)-exp(-lambda*L)) + exp(-lambda*L))/lambda;
}

//! Density function
//! pdf for the (L,U)-truncated Exponential distribution
real math::random_gsl::pdfExponentialLU(const real x, const real lambda, const real L, const real U) {
    return lambda*exp(-lambda*x)/(exp(-lambda*L)-exp(-lambda*U));
}

unsigned int math::random_gsl::rndPoisson(const real lambda) {
	return gsl_ran_poisson(randGen,lambda);
}

//! Bernoulli random number generator
unsigned int math::random_gsl::rndBernoulli(const real p) {
    return (rndUniform()<=(1.0-p))?0:1;
}

//! Random index
unsigned int math::random_gsl::rndIndex(const _vector<real> &w, bool normalized) {
    real u, acum;
    unsigned int i=0;
    u= (normalized)?rndUniform():(rndUniform()*w.sum());
    acum= w[i];
    while (u>acum) acum+= w[++i];
    return i;
}

//! MH test
bool math::random_gsl::acceptedMH(const real ratio) {
    if (ratio>=1.0) {
        return true;
    }
    return rndBernoulli(ratio);
}

#endif // my_RANDOM_GSL_HPP
