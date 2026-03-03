#ifndef FUNCTIONAL_DATA_MISSING_MCMC_HPP
#define FUNCTIONAL_DATA_MISSING_MCMC_HPP

#include "datatypes.h"
#include "matrix.hpp"
#include "membership.hpp"
#include "vector.hpp"
#include "random.hpp"
#include "normal_gamma_product_missing.hpp"

typedef datamodels::normal_normalgamma_product_missing datamodelMissing;
#ifdef USE_R_RGN
#include <Rcpp.h>
#endif

class  functional_data_missing_mcmc {
protected:
	//! Latest values
	typename datamodelMissing::container_parameter c_th;
	_vector<unsigned int> c_d;
	_vector<unsigned int> c_N;

	//! Containers
	typename datamodelMissing::container_parameter th;
	_matrix<unsigned int> d, N;

	//! Additional
	_vector<unsigned int> maxTh;	///< Number of th's at each iteration
	membership mem;			///< To update th

	_matrix<unsigned int> pp; // partition
	_vector<unsigned int> kk; // number of blocks

	real w0, hpR, hpA, hpB;  //! Value for r and Beta prior parameters (a,b)
	real c_ups;          //! Latest simulated value for p
	_vector<real> ups;   //! Container for p

	datamodelMissing dm;			///< Data model


	void update_theta() {
			unsigned int i;
			for (i=0; i<c_th.size(); i++) {
				_vector<unsigned int> indices= mem.members(i);
				if (indices.size()==0) {
					c_th.set(i,dm.rndP0());
				} else {
					c_th.set(i,dm.rndPosterior(indices));
				}
			}

			c_th.resize(c_N.max());
			for (; i<c_th.size(); i++) {
				c_th.set(i,dm.rndP0());
			}
	}

	void update_d() {	/// d>=1
		_vector<real> ps;
		unsigned int i, k, di;

		mem.clear();
		for (i=0; i<dm.y.size(); i++) {
			ps.resize(c_N[i]);
			for (k=0; k<c_N[i]; k++) {
				ps[k]= dm.K(i,c_th[k]);
			}
			di= _r::rndIndex(ps);
			c_d[i]= di+1;
			mem.add(di,i);
		}
	}

	void update_N() {
		for (unsigned int i=0; i<dm.y.size(); i++) {
			c_N[i]= _r::rndPoisson((1.0-c_ups) * _r::rndBeta(real(c_N[i]-c_d[i]+1),real(c_d[i]))
			      * _r::rndGamma(c_N[i]+hpR-1.0,1.0)) + c_d[i];
		}
	}

	void update_upsilon() {
		c_ups= _r::rndBeta(hpA + hpR*dm.y.size(), hpB + real(c_N.sum()-dm.y.size()));
	}

	//! @param components [in] Initial number of components N_i
	void init(unsigned int components) {
		c_N.resize(dm.y.size());
		c_d.resize(dm.y.size());
		c_th.resize(components);

		unsigned int i, di;
		for (i=0; i<components; i++) {
			c_th.set(i,dm.rndP0());
		}
		for (i=0; i<dm.y.size(); i++) {
			c_N[i]= components;
			di= _r::rndDUniform(1,components);
			c_d[i]= di;
			mem.add(di-1,i);
		}

		dm.update_missing(c_th,c_d);

		if (w0<=0.0 || w0>1.0) c_ups= hpA/(hpA+hpB);
		else c_ups= w0;
	}

  void iterate() {
		dm.update_missing(c_th,c_d);
    update_N();
		update_theta();
		update_d();
    update_upsilon();
  }

	//! @param iterations [in] number of iterations
	//! @param lag [in] lag of each iteration
	void burn(unsigned int iterations, unsigned int lag= 1) {
		unsigned int i, I= iterations*lag;

		for (i=0; i<I; i++) {
      iterate();
		}
		COLLECT_GARBAGE;
	}

	//! @param iterations [in] number of iterations
	//! @param lag [in] lag of each iteration
	void sample(unsigned int iterations, unsigned int lag= 1) {
		th.resize((c_th.size()+10)*iterations);

		d.resize(iterations,dm.y.size());
		N.resize(iterations,dm.y.size());
		pp.resize(iterations,dm.y.size());

		maxTh.resize(iterations);
		kk.resize(iterations);
		ups.resize(iterations);

		dm.resize(iterations);

		_vector<unsigned int> c_pp;
		c_pp.resize(dm.y.size());

		unsigned int i, j, k, I=0;
		for (i=0; i<iterations; i++) {
			j=0;
			do {
        iterate();
			} while ((++j)<lag);

			dm.store(i);

			c_pp= mem.partition();
			for (k=0; k<dm.y.size(); k++) {
				d(i,k)= c_d[k];
				N(i,k)= c_N[k];
				pp(i,k)= c_pp[k];
			}

      c_pp= mem.order();
			for (k=0; k<c_th.size(); k++, I++) {
				th.set(I,c_th[k]);
			}

			maxTh[i]= c_th.size();
			kk[i]= mem.size();
      ups[i]= c_ups;
		}
		th.resize(I);
		COLLECT_GARBAGE;
	}

public:
	functional_data_missing_mcmc(const datamodelMissing &dataModel) : dm(dataModel) { }

	//! @param hp [in] vector containing the value for r and hyperparameters for p as follows: (r,a,b)
	void setHyperParameters(const _vector<real> &hp) {
		w0= hp[0];
		hpR= hp[1];
		hpA= hp[2];
		hpB= hp[3];
	}

  void run(unsigned int it_burn, unsigned int it_sample, unsigned int lag = 1, unsigned int n0 = 10) {
    init(n0);
	  burn(it_burn,lag);
    sample(it_sample,lag);
  }

#ifdef USE_R_RGN
  void saveR(Rcpp::NumericMatrix &Rth, Rcpp::IntegerMatrix &Rd, Rcpp::IntegerMatrix &RN, Rcpp::IntegerVector &RNmax, Rcpp::IntegerMatrix &Rp, Rcpp::IntegerVector &Rk, Rcpp::NumericVector &Rl, Rcpp::IntegerMatrix &RidxMis, Rcpp::NumericMatrix &RyMis) {
    Rcpp::NumericMatrix mt(th.rows(),th.cols());
    Rcpp::IntegerMatrix mp(pp.rows(),pp.cols()), md(d.rows(),d.cols()), mN(d.rows(),d.cols());
    Rcpp::IntegerVector vmax(maxTh.size()), vk(kk.size());
    Rcpp::NumericVector vl(ups.size());

		_matrix<unsigned int> idxM= dm.indices_missing();
		_matrix<real> yM= dm.sampled_missing();
		Rcpp::IntegerMatrix idxMis(idxM.rows(),idxM.cols());
		Rcpp::NumericMatrix yMis(yM.rows(),yM.cols());


    for (unsigned int i=0; i<vk.size(); i++) {
      vmax[i]= maxTh[i];
      vk[i]= kk[i];
      vl[i]= ups[i];
      for (unsigned int j=0; j<pp.cols(); j++) {
        md(i,j)= d(i,j);
        mN(i,j)= N(i,j);
        mp(i,j)= pp(i,j);
      }
    }

    for (unsigned int i=0; i<th.rows(); i++) {
      for (unsigned int j=0; j<th.cols(); j++) {
        mt(i,j)= th(i,j);
      }
    }

		for (unsigned int j=0; j<yMis.cols(); j++) {
			idxMis(j,0)= idxM(j,0)+1;
			idxMis(j,1)= idxM(j,1)+1;
			for (unsigned int i=0; i<yMis.rows(); i++) {
				yMis(i,j)= yM(i,j);
			}
		}

    Rth= mt;
    Rd= md;
    RN= mN;
    RNmax= vmax;
    Rp= mp;
    Rk= vk;
    Rl= vl;
		RidxMis= idxMis;
		RyMis= yMis;
  }
#endif
};

#endif // FUNCTIONAL_DATA_MISSING_MCMC_HPP
