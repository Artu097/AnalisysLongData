#ifndef DATA_MODEL_NG_PRODUCT_MISSING_HPP
#define DATA_MODEL_NG_PRODUCT_MISSING_HPP

#include "normal_gamma.hpp"

namespace datamodels {

inline unsigned int NNGprodMis_dim = 1;

class normal_normalgamma_product_missing {
public:
	typedef _matrix<real> data;
	typedef _matrix<real> parameter;

	class container_parameter : public _matrix<real> {
// 		normal_normalgamma *cc;

	public:
		container_parameter() {
			_matrix<real>::setCols(2);
// 			cc= new normal_normalgamma[datamodels::NNGprodMis_dim];
		}

		inline void resize(unsigned int n) {
			_matrix<real>::resize(datamodels::NNGprodMis_dim*n);
		}

		inline void set(unsigned int i, const parameter &v) {
			for (unsigned int l=0; l<datamodels::NNGprodMis_dim; l++) {
				_matrix<real>::operator()(i*datamodels::NNGprodMis_dim+l,0)=v(l,0);
				_matrix<real>::operator()(i*datamodels::NNGprodMis_dim+l,1)=v(l,1);
			}
		}

		parameter operator[](unsigned int i) const {
			parameter vv(datamodels::NNGprodMis_dim,2);
			for (unsigned int l=0; l<datamodels::NNGprodMis_dim; l++) {
				vv(l,0)= _matrix<real>::operator()(i*datamodels::NNGprodMis_dim+l,0);
				vv(l,1)= _matrix<real>::operator()(i*datamodels::NNGprodMis_dim+l,1);
			}
			return vv;
		}

		inline unsigned int size() const {
			return rows()/datamodels::NNGprodMis_dim;
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
	// _matrix<short unsigned int> mm;
	// _vector<unsigned int> totMisCol;
	_matrix<unsigned int> ijm;
	_matrix<real> yijm;
	unsigned int nn;

public:
	data y; /// TODO: Create an interface for this variable and hide it

// 	normal_normalgamma_product() : M(0) { }

// 	void setDimension(unsigned int m) {
// 		datamodels::NNGprodMis_dim= m;
// 		cc= new normal_normalgamma[m];
// 	}

	void setData(const data &y0) {
		datamodels::NNGprodMis_dim= y0.cols();
		cc= new normal_normalgamma[y0.cols()];
		y.resize(y0.rows(),y0.cols());
		// mm.resize(y0.rows(),y0.cols());
		// totMisCol.resize(y0.cols());
		nn= y0.rows();
		ijm.resize(0,2);
		unsigned int I=0;
		for (unsigned int j=0; j<datamodels::NNGprodMis_dim; j++) {
			cc[j].y.resize(nn);
			// totMisCol[j]= 0;
			for (unsigned int i=0; i<nn; i++) {
				cc[j].y[i]= y0(i,j);
				if (std::isnan(y0(i,j))) {
					// cc[j].y[i]= 0.0;
					ijm(I,0)= i;
					ijm(I,1)= j;
					// std::cout<<ijm(I,0)<<"x"<<ijm(I,1)<<"\n";
					I++;
					// mm(i,j)= 1;
					// totMisCol[j]++;
				// } else {
					// mm(i,j)= 0;
				// } else {
					// cc[j].y[i]= y0(i,j);
				}
			}
			// std::cout<<"Missing values at column "<<j<<": "<<totMisCol[j]<<"\n";
		}

		// std::cout<<"DATA\n";
		// for (unsigned int i=0; i<nn; i++) {
		// 	for (unsigned int j=0; j<datamodels::NNGprodMis_dim; j++) {
		// 		std::cout<<cc[j].y[i]<<" ";
		// 	}
		// 	std::cout<<"\n";
		// }
	}

	void setBaseMeasureParameters(const _vector<real> &m, const _vector<real> &c, const _vector<real> &a, const _vector<real> &b) {
		for (unsigned int j=0; j<datamodels::NNGprodMis_dim; j++) {
			cc[j].setBaseMeasureParameters(m[j],c[j],a[j],b[j]);
		}
	}

	//! @param m [in] Base measure parameter
	//! @param c [in] Base measure parameter
	//! @param a [in] Base measure parameter
	//! @param b [in] Base measure parameter
	void setBaseMeasureParameters(real m, real c, real a, real b) {
		for (unsigned int j=0; j<datamodels::NNGprodMis_dim; j++) {
			cc[j].setBaseMeasureParameters(m,c,a,b);
		}

	}

	//! @return random number from P0
	inline parameter rndP0() {
		parameter t(datamodels::NNGprodMis_dim,2);
		_vector<real> vt(2);
// std::cout<<"rand P0\n";
		for (unsigned int j=0; j<datamodels::NNGprodMis_dim; j++) {
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
		parameter t(datamodels::NNGprodMis_dim,2);
		_vector<real> vt(2);
		for (unsigned int j=0; j<datamodels::NNGprodMis_dim; j++) {
			vt= cc[j].rndPosterior(indices);
			t(j,0)= vt[0];
			t(j,1)= vt[1];
		}
		return t;
	}

  inline real logK(const _vector<real> &x, const parameter &parameters) {
		real k0=0.0;
		for (unsigned int j=0; j<datamodels::NNGprodMis_dim; j++) {
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
		for (unsigned int j=0; j<datamodels::NNGprod_dim; j++) {
			k0+= cc[j].logK(index,parameters(j,0),parameters(j,1));
		}
		return k0;
	}

	inline real K(unsigned int index, const parameter &parameters) {
    return exp(logK(index,parameters));
  }

	// void update_missing(const parameter &theta, const _vector<unsigned int> &d) {
	// 	_vector<real> pM(2);
	// 	for (unsigned int j=0; j<datamodels::NNGprodMis_dim; j++) {
	// 		if (totMisCol[j]==0) {
	// 			continue;
	// 		}
	// 		for (unsigned int i=0; i<nn; i++) {
	// 			if (mm(i,j)==1) {
	// 				pM= theta[d[i]][j];
	// 				cc[j].y[i]= _r::rndNormal(pM[0],1.0/pM[1]);
	// 				// cc[j].y[i]= _r::rndNormal(theta[d[i]][j][0],1.0/theta[d[i]][j][1]);
	// 			}
	// 		}
	// 	}
	// }
	void update_missing(const parameter &theta, const _vector<unsigned int> &d) {
		_vector<real> pM(2);
		// std::cout<<"|theta|="<<theta.rows()<<" x "<<theta.cols()<<"\n";
		// for (unsigned int i=0; i<theta.rows(); i++) {
		// 	for (unsigned int j=0; j<theta.cols(); j++) {
		// 		std::cout<<theta[i][j]<<" ";
		// 	}
		// 	std::cout<<"\n";
		// }

		for (unsigned int l=0; l<ijm.rows(); l++) {
			pM= theta[datamodels::NNGprodMis_dim*(d[ijm(l,0)]-1)+ijm(l,1)]; //,ijm(l,1));
			// std::cout<<"actualizando con d[]: "<<d[ijm(l,0)]<<" []:"<<ijm(l,1)<<"\n";
			// std::cout<<datamodels::NNGprodMis_dim*(d[ijm(l,0)]-1)+ijm(l,1)<<" "<<ijm(l,1)<<"\n";
			// std::cout<<"param: "<<pM<<"\t\t";
			// std::cout<<theta(datamodels::NNGprodMis_dim*(d[ijm(l,0)]-1)+ijm(l,1),ijm(l,1))<<"\n";
			cc[ijm(l,1)].y[ijm(l,0)]= _r::rndNormal(pM[0],1.0/pM[1]);
			// std::cout<<ijm(l,0)<<" , "<<ijm(l,1)<<"\t";
			// std::cout<<cc[ijm(l,1)].y[ijm(l,0)]<<"\n";
		}
	}

	// For storing missing values' samples, memory allocation
	void resize(unsigned int n) {
		yijm.resize(n,ijm.rows());
	}

	// Storing missing values' samples
	void store(unsigned int i) {
		for (unsigned int l=0; l<ijm.rows(); l++) {
			yijm(i,l)= cc[ijm(l,1)].y[ijm(l,0)];
		}
	}

	// Retrieving sampled values
	_matrix<unsigned int> indices_missing() const {
		return ijm;
	}

	_matrix<real> sampled_missing() const {
		return yijm;
	}

}; // normal_normalgamma

} // datamodels

#endif // DATA_MODEL_NG_PRODUCT_MISSING_HPP

