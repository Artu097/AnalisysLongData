/*! @file membership.hpp
 *  Class implementing membership variables management
 *  Assumptions: the minimum index is always zero and indices are always consecutive
 */

#ifndef my_MEMBERSHIP_HPP
#define my_MEMBERSHIP_HPP

#include "vector.hpp"

class membership {
	_vector<unsigned int> mm; //! values for uniques
	_vector< _vector<unsigned int> > ii; //! vector of indices for each unique
	unsigned int total;

public:

	membership();

	//! @param indices [in] reference vector to create memberships
	membership(const _vector<unsigned int> &indices);

	//! @param member [in] unique value
	//! @param index [in] new index belonging to the unique value `member'
	void add(unsigned int member, unsigned int index);

	//! @return number of unique values
	inline unsigned int size() const { return mm.size(); }

	//! @param i [in] index of the unique value
	//! @return the ith unique value or -1 if i is out the range
	unsigned int member(unsigned int i) const;

	//! @param member [in] unique value
	//! @return vector with indices for the unique value `member';
	//!         if `member' does not belong to mm, an empty vector is returned
	_vector<unsigned int> members(unsigned int member) const;

	//! @return vector of groups' frequencies
	_vector<unsigned int> frequencies() const;

	//! @param index [in]
	//! @return the index of the unique value where index is associated
	unsigned int where(unsigned int index) const;


	//! @param index [in] index to be exchanged
	//! @param member [in] unique value for index
	void swap(unsigned int index, unsigned int member);

	void show();

	_vector<unsigned int> partition();
	_vector<unsigned int> natural();
	_vector<unsigned int> order();

	void clear();
};

/* *********************************************************************** */
/* *********************************************************************** */

inline membership::membership() : total(0) {}

inline membership::membership(const _vector<unsigned int> &indices) {
	total=0;
	for (unsigned int i=0; i<indices.size(); i++) {
		add(indices[i],i);
	}
}

inline void membership::add(unsigned int member, unsigned int index) {
	unsigned int i, k;
	for (i=0; i<mm.size(); i++) {
		if (mm[i]==member) break;
	}
	if (i==mm.size()) {
		mm[i]= member;
		ii.append(1);
		ii[i].resize(1);
		k=0;
	} else {
		k= ii[i].rows();
	}
	ii[i][k]= index;
	total++;
}

inline unsigned int membership::member(unsigned int i) const {
  if (i>=mm.size()) return (unsigned int)-1;
	return mm[i];
}

inline _vector<unsigned int> membership::members(unsigned int member) const {
	unsigned int i;
	for (i=0; i<mm.size(); i++) {
		if (mm[i]==member) return ii[i];
	}
	return _vector<unsigned int>(0);
}

inline void membership::swap(unsigned int index, unsigned int member) {
	unsigned int i, j, m, foundI=0, foundM=0;
	for (j=0; j<ii.size(); j++) {
		for (i=0; i<ii[j].size(); i++) {
			if (ii[j][i]==index) {
				foundI= 1;
				break;
			}
		}
		if (foundI) {
			break;
		}
	}

	for (m=0; m<ii.size(); m++) {
		if (mm[m]==member) {
			foundM= 1;
			break;
		}
	}

	// std::cout<<"foundI "<<foundI<<" foundM "<<foundM<<std::endl;
	// std::cout<<"i "<<i<<" j "<<j<<"  m "<<m<<std::endl;

	if (foundI) {
		if (foundM) {
			if (j!=m) {
				ii[m][ii[m].size()]= index;
				if (ii[j].size()==1) {
					ii.pop(j);
					mm.pop(j);
				} else {
					ii[j].pop(i);
				}
			}
		} else {
			if (ii[j].size()==1) {
				mm[j]= member;
			} else {
				ii[j].pop(i);
				ii.append(1);
				ii[ii.size()-1].resize(1);
				ii[ii.size()-1][0]= index;
				mm.append(1);
				mm[mm.size()-1]= member;
			}
		}
	} else {
		if (foundM) {
			ii[m][ii[m].size()]= index;
		} else {
			ii.append(1);
			ii[ii.size()-1].resize(1);
			ii[ii.size()-1][0]= index;
			mm.append(1);
			mm[mm.size()-1]= member;
		}
	}
}

inline _vector<unsigned int> membership::frequencies() const {
	_vector<unsigned int> nj(ii.size());
	for (unsigned int i=0; i<nj.size(); i++) {
		nj[i]= ii[i].size();
	}
	return nj;
}

inline unsigned int membership::where(unsigned int index) const {
	for (unsigned int i=0; i<ii.size(); i++) {
		for (unsigned int j=0; j<ii[i].size(); j++) {
			if (ii[i][j]==index) {
				return i;
			}
		}
	}
	return -1;
}

inline void membership::show() {
	std::cout<<"k= "<<mm.size()<<"\n";
	for (unsigned int i=0; i<mm.size(); i++) {
		std::cout<<"\tvalue "<<mm[i]<<"\n\t\t";
		for (unsigned int j=0; j<ii[i].size(); j++) {
			std::cout<<"  "<<ii[i][j];
		}
		std::cout<<"\n";
	}
}

inline _vector<unsigned int> membership::partition() {
	unsigned int i, j, l=0, c=1;
	_vector<unsigned int> pp(total);
	for (i=0; i<total; i++) pp[i]= 0;

	while (l<total) {
		i= where(l);
		for (j=0; j<ii[i].size(); j++) {
			pp[ii[i][j]]= c;
		}
		c++;
		while (pp[l]!=0) {
			l++;
			if (l==total) break;
		}
	}

	return pp;
}

inline _vector<unsigned int> membership::natural() {
	_vector<unsigned int> nn(total);
	for (unsigned int i=0; i<ii.size(); i++) {
		for (unsigned int j=0; j<ii[i].size(); j++) {
			nn[ii[i][j]]= i;
		}
	}
	return nn;
}

inline _vector<unsigned int> membership::order() {
	_vector<unsigned int> idx(ii.size()),idxO(ii.size()), res(ii.size());
	unsigned int i0;

	for (unsigned int i=0; i<idx.size(); i++) {
		idx[i]= ii[i].min();
		idxO[i]= idx[i];
	}
	idxO.sort();

	for (unsigned int i=0; i<idx.size(); i++) {
		for (unsigned int j=0; j<idx.size(); j++) {
			if (idx[i]==idxO[j]) {
				res[j]=i;
				break;
			}
		}
	}

	return res;
}

inline void membership::clear() {
	mm.resize(0);
	ii.resize(0);
	total=0;
}


#endif // my_MEMBERSHIP_HPP
