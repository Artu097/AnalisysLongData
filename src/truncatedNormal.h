#ifndef TRUNC_NORMAL_H
#define TRUNC_NORMAL_H

#include "vector.hpp"
#include "random.hpp"

// (minX,maxX) truncated normal with mean m and variance v
real rndTruncNorm(real m, real v, real minX, real maxX) {
	real u0, rho, minV, maxV;
	minV= (minX - m)/sqrt(v);
	maxV= (maxX - m)/sqrt(v);
	do {
		u0= _r::rndUniform(minV,maxV);
		if (maxV<0.0) {
			rho= exp((maxV*maxV-u0*u0)/2.0);
		} else if (0.0<minV) {
			rho= exp((minV*minV-u0*u0)/2.0);
		} else {
			rho= exp((-u0*u0)/2.0);
		}
	} while(_r::rndUniform()>rho);
	
	return m + u0*sqrt(v); 
}

real rndTruncNorm(const _vector<real> &th, real minX, real maxX) {
	return rndTruncNorm(th[0],th[1],minX,maxX);
}

real rndLeftTruncNorm(real m, real v, real minX) {
	real x, al= (minX-m)/sqrt(v);
    if (al < 0.45) {
		do {
            x= _r::rndNormal(0,1);
		} while(x<al);
	} else {
		real ainv= 1.0/al;
		do {
            x= _r::rndExponential(ainv) + al;
		} while (_r::rndUniform() > exp(-(x-al)*(x-al)/2));
	}
    return m + x*sqrt(v);
}

real rndLeftTruncNorm(const _vector<real> &th, real minX) {
	return rndLeftTruncNorm(th[0],th[1],minX);
}

real cdfLeftTruncNorm(real x, real m, real v, real minX, int logp=0) {
#ifdef USE_R_RGN
	return R::pnorm(x,m,sqrt(v),1,logp);
#endif
}

#endif // TRUNC_NORMAL_H
