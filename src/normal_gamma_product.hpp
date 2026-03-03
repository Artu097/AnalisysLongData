#ifndef DATA_MODEL_NG_PRODUCT_HPP
#define DATA_MODEL_NG_PRODUCT_HPP

#include "normal_gamma.hpp"

namespace datamodels {

unsigned inline int NNGprod_dim=1;

class normal_normalgamma_product {
public:
	typedef _matrix<real> data;
	typedef _matrix<real> parameter;

	class container_parameter : public _matrix<real> {
// 		normal_normalgamma *cc;

	public:
		container_parameter() {
			_matrix<real>::setCols(2);
// 			cc= new normal_normalgamma[datamodels::NNGprod_dim];
		}

		inline void resize(unsigned int n) {
			_matrix<real>::resize(datamodels::NNGprod_dim*n);
		}

		inline void set(unsigned int i, const parameter &v) {
			for (unsigned int l=0; l<datamodels::NNGprod_dim; l++) {
				_matrix<real>::operator()(i*datamodels::NNGprod_dim+l,0)=v(l,0);
				_matrix<real>::operator()(i*datamodels::NNGprod_dim+l,1)=v(l,1);
			}
		}

		parameter operator[](unsigned int i) const {
			parameter vv(datamodels::NNGprod_dim,2);
			for (unsigned int l=0; l<datamodels::NNGprod_dim; l++) {
				vv(l,0)= _matrix<real>::operator()(i*datamodels::NNGprod_dim+l,0);
				vv(l,1)= _matrix<real>::operator()(i*datamodels::NNGprod_dim+l,1);
			}
			return vv;
		}

		inline unsigned int size() const {
			return rows()/datamodels::NNGprod_dim;
		}

		void save(const text &path, const text &preffix="", const text &suffix="", bool binary=true) {
			text p=path, ext=".dat";
			if (path=="") p="./";
			else if (*(path.end()-1)!='/') p+="/";
			_matrix<real>::save(text(p+preffix+"theta"+suffix+ext).c_str(),binary);
		}
	};

private:
	/// TODO: A better option might be using to typedefs, one for bb and the other for pp.
// 	parameter bb, pp;

	normal_normalgamma *cc;

public:
	data y; /// TODO: Create an interface for this variable and hide it

// 	normal_normalgamma_product() : M(0) { }

// 	void setDimension(unsigned int m) {
// 		datamodels::NNGprod_dim= m;
// 		cc= new normal_normalgamma[m];
// 	}

	void setData(const data &y0) {
		datamodels::NNGprod_dim= y0.cols();
		cc= new normal_normalgamma[y0.cols()];
		y.resize(y0.rows(),y0.cols());
		for (unsigned int j=0; j<datamodels::NNGprod_dim; j++) {
			cc[j].y.resize(y0.rows());
// std::cout<<"[ ";
			for (unsigned int i=0; i<y0.rows(); i++) {
// std::cout<<y0(i,j)<<" ";
				cc[j].y[i]= y0(i,j);
			}
// std::cout<<" ]\ndata: "<<j<<"-> ("<<cc[j].y<<")\n";
		}
	}

	void setBaseMeasureParameters(const _vector<real> &m, const _vector<real> &c, const _vector<real> &a, const _vector<real> &b) {
		for (unsigned int j=0; j<datamodels::NNGprod_dim; j++) {
			cc[j].setBaseMeasureParameters(m[j],c[j],a[j],b[j]);
		}
	}

	//! @param m [in] Base measure parameter
	//! @param c [in] Base measure parameter
	//! @param a [in] Base measure parameter
	//! @param b [in] Base measure parameter
	void setBaseMeasureParameters(real m, real c, real a, real b) {
		for (unsigned int j=0; j<datamodels::NNGprod_dim; j++) {
			cc[j].setBaseMeasureParameters(m,c,a,b);
		}

	}

	//! @return random number from P0
	inline parameter rndP0() {
		parameter t(datamodels::NNGprod_dim,2);
		_vector<real> vt(2);
// std::cout<<"rand P0\n";
		for (unsigned int j=0; j<datamodels::NNGprod_dim; j++) {
			vt= cc[j].rndP0();
			t(j,0)= vt[0];
			t(j,1)= vt[1];
// std::cout<<vt<<"\n";
		}
		return t;
	}

	//! @param indices [in] subset of y
	//! @return random number from the 'restricted' posterior distribution
	parameter rndPosterior(_vector <unsigned int> &indices) {
		parameter t(datamodels::NNGprod_dim,2);
		_vector<real> vt(2);
// std::cout<<"rand posterior ("<<indices<<")\n";
		for (unsigned int j=0; j<datamodels::NNGprod_dim; j++) {
			vt= cc[j].rndPosterior(indices);
			t(j,0)= vt[0];
			t(j,1)= vt[1];
// std::cout<<vt<<"\n";
		}
		return t;
	}

  inline real logK(const _vector<real> &x, const parameter &parameters) {
		real k0=0.0;
// std::cout<<"***** eval kernel at "<<x<<"\n";
		for (unsigned int j=0; j<datamodels::NNGprod_dim; j++) {
// std::cout<<"      "<<j<<" "<<parameters(j,0)<<","<<parameters(j,1)<<"\n";
			k0+= cc[j].logK(x[j],parameters(j,0),parameters(j,1));
		}

		return k0;
  }

	//! @param x [in] point to be evaluated
	//! @param parameters [in] parameters for K
	//! @return pdf K(x;parameters)
	inline real K(const _vector<real> &x, const parameter &parameters) {
    return exp(logK(x,parameters));
	}

	//! @param index [in] index of the data point to be evaluated
	//! @param parameters [in] parameters for K
	//! @return pdf K(y_index;parameters)
	inline real logK(unsigned int index, const parameter &parameters) {
		real k0=0.0;
// std::cout<<"***** eval kernel at index "<<index<<"\n";
		for (unsigned int j=0; j<datamodels::NNGprod_dim; j++) {
// std::cout<<"      "<<cc[j].y[index]<<" | "<<parameters(j,0)<<","<<parameters(j,1)<<"\n";
			k0+= cc[j].logK(index,parameters(j,0),parameters(j,1));
		}

		return k0;
	}

	inline real K(unsigned int index, const parameter &parameters) {
    return exp(logK(index,parameters));
  }

}; // normal_normalgamma

} // datamodels

#endif // DATA_MODEL_NG_PRODUCT_HPP

