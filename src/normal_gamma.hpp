#ifndef DATA_MODEL_NG_HPP
#define DATA_MODEL_NG_HPP

namespace datamodels {

class normal_normalgamma {
public:
	typedef _vector<real> data;
	typedef _vector<real> parameter;

	class container_parameter : public _matrix<real> {
	public:
		container_parameter() {
			_matrix<real>::setCols(2);
		}

		inline void resize(unsigned int n) {
			_matrix<real>::resize(n);
		}

		inline void set(unsigned int i, const parameter &v) {
			_matrix<real>::operator()(i,0)=v[0];
			_matrix<real>::operator()(i,1)=v[1];
		}

		parameter operator[](unsigned int i) const {
			parameter vv(2);
			vv[0]= _matrix<real>::operator()(i,0);
			vv[1]= _matrix<real>::operator()(i,1);
			return vv;
		}

		inline unsigned int size() const {
			return rows();
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
	parameter bb, pp;

public:
	data y; /// TODO: Create an interface for this variable and hide it

	void setData(const data &y0) {
		y=y0;
	}

	//! @param m [in] Base measure parameter
	//! @param c [in] Base measure parameter
	//! @param a [in] Base measure parameter
	//! @param b [in] Base measure parameter
	void setBaseMeasureParameters(real m, real c, real a, real b) {
		bb.resize(4);

		bb[0]= m;
		bb[1]= c;
		bb[2]= a;
		bb[3]= b;

		calculatePosteriorParameters();
	}

	void setBaseMeasureParameters(const parameter &base) {
		setBaseMeasureParameters(base[0],base[1],base[2],base[3]);
	}

	void readParameters(std::istream &input) {
		for (unsigned int i=0; i<4; i++) {
			input>>bb[i];
		}
	}

	void calculatePosteriorParameters() {
		pp.resize(4);

		real sy, sy2, nj;
		sy= y.sum();
		sy2= y.sum2();
		nj= y.rows();

		pp[0]= (bb[1]*sy+bb[0])/(bb[1]*nj+1.0);
		pp[1]= bb[1]/(bb[1]*nj+1.0);
		pp[2]= nj/2.0 + bb[2];
		pp[3]= (sy2-sy*sy/nj)/2.0 + nj*(sy/nj-bb[0])*(sy/nj-bb[0])/(2.0*bb[1]*nj+2.0) + bb[3];
	}

	//! @return random number from P0
	inline parameter rndP0() {
		return _r::rndNormalGamma(bb);
	}

	//! @return random number from the posterior distribution
	inline parameter rndPosterior() {
		return _r::rndNormalGamma(pp);
	}

	//! @param indices [in] subset of y
	//! @return random number from the 'restricted' posterior distribution
	parameter rndPosterior(_vector <unsigned int> &indices) {
		parameter ppi(4);

		real sy, sy2, nj;
		sy= y.sum(indices);
		sy2= y.sum2(indices);
		nj= indices.rows();

		ppi[0]= (bb[1]*sy+bb[0])/(bb[1]*nj+1.0);
		ppi[1]= bb[1]/(bb[1]*nj+1.0);
		ppi[2]= nj/2.0 + bb[2];
		ppi[3]= (sy2-sy*sy/nj)/2.0 + nj*(sy/nj-bb[0])*(sy/nj-bb[0])/(2.0*bb[1]*nj+2.0) + bb[3];

	 	return _r::rndNormalGamma(ppi);
	}

	//! @param x [in] point to be evaluated
	//! @param parameters [in] parameters for K
	//! @return pdf K(x;parameters)
	inline real K(real x, const parameter &parameters) {
		return _r::pdfNormal(x,parameters[0],1.0/parameters[1]);
	}

	//! @param index [in] index of the data point to be evaluated
	//! @param parameters [in] parameters for K
	//! @return pdf K(y_index;parameters)
	inline real K(unsigned int index, const parameter &parameters) {
		return _r::pdfNormal(y[index],parameters[0],1.0/parameters[1]);
	}

	inline real logK(real x, real mu, real tau) {
		return (log(tau)-log(2.0*M_PI))/2.0 - tau*(x-mu)*(x-mu)/2.0;
	}

	inline real logK(unsigned int index, real mu, real tau) {
		return logK(y[index],mu,tau);
	}


}; // normal_normalgamma

} // datamodels

#endif // DATA_MODEL_NG_HPP

