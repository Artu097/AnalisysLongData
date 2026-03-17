/*! @file vector.hpp
 *  Template class to use vector structure; used mainly to store data
 */

#ifndef my_VECTOR_HPP
#define my_VECTOR_HPP

#include "memalloc.hpp"
#include "datatypes.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <ios>
#include <limits>

namespace structures {

template <typename T> class matrix;

///-----------------------------------------------------------------------------
/// Vector class; declaration
///-----------------------------------------------------------------------------

/*! @class vector<T>
 *  This template class creates arrays that stores data of type/class T
 *
 *  It wouldn't be used as math vector because arithmetic and logic operations
 *  are not implemented
 */
template <typename T>
class vector {

protected:

    //! @brief Vector's data structure
    T *dd;	///< pointer which will store data
    unsigned int rr,	///< number of rows
	 avmem;	///< maximum number of rows available in dd
	 bool rowMatrix;

public:

    /// Constructors

    //! @brief Default constructor
    vector();

    //! @brief Another constructor
    //! @param rows [in] number of rows
    vector(unsigned int rows);

    //! @brief Copy constructor
    //! @param v [in] reference vector
    vector(const vector<T>& v);

	vector(const T *arr, unsigned int n);

    //! @brief Destructor
    ~vector();

    //! @brief Returns the number of rows of the vector
    inline unsigned int rows() const { return rr; }
    inline unsigned int size() const { return rr; }

    //! @brief Assignment operator
    //! @param v [in] vector with the new contents
    vector<T>& operator=(const vector<T>& v);

    //! @brief Comparison operators
    //! @param v [in] comparing vector
    bool operator==(const vector<T>& v) const;
    bool operator!=(const vector<T>& v) const { return !((*this)==v); }

    //! @brief Access to elements
    //! @param i [in] According to its value some of the following actions will be done
    //!   0<=i<rows.	Returns/sets the (i+1)-th elements
    //!   i>=rows.	Resizes the vector at i+1 rows and sets the last element to the given one
    T& operator[](unsigned int i);
    T operator[](unsigned int i) const;

    //! @brief Returns the vector without the (i+1)-th element
    //! @param i [in] Element entry (+1) to be eliminated
    vector<T> without(unsigned int i);

    /// Functions which modify the structure

    //! @brief Resizes vector
    //! @param rows [in] new number of rows
    void resize(unsigned int rows);

    //! @brief Appends rows at the end of the vector
    //! @param rows [in] number of rows to be appended; if rows<0 the last -rows will be deleted
    void append(sint rows);

    //! @brief Deletes the ith row
    //! @param i [in] index of the row to be deleted
    void pop(unsigned int i);

    /// Misc functions

    //! @brief Return minimum and maximum
    T min() const;
    T min(const vector<unsigned int> &indices) const;

    T max() const;
    T max(const vector<unsigned int> &indices) const;

    //! @brief Returns the sum of all the elements
    T sum() const;
    T sum(const vector<unsigned int> &indices) const;

    //! @brief Returns the sum of squaresS
    T sum2() const;
    T sum2(const vector<unsigned int> &indices) const;

    //! @brief Sorts the vector
    void sort();

    template <typename t> friend class matrix;

private:

    //! @brief Sorting function
    //! @param *w [in] pointer to vector which will be sorted
    //! @param  n [in] size of the vector
    void shellsort(T *w, unsigned int n);

public:

    static char eoe[5]; ///< text which indicates the end of each element

    //! @brief Overloaded operators for I/O
    template <typename t>
    friend std::ostream& operator<<(std::ostream &os, const vector<t> &v);

	template <typename t>
    friend std::istream& operator>>(std::istream &is, vector<t> &v);

    void saveBinary(const char* name);
    void loadBinary(const char* name);

    void saveText(const char* name);
	void loadText(const char* name);

	inline void save(const char* name, bool binary=true) {
		if (binary) saveBinary(name);
		else saveText(name);
	}

	inline void load(const char* name, bool binary=true) {
		if (binary) loadBinary(name);
		else loadText(name);
	}

	void copyArray(const T* arr, unsigned int n);
}; // END class vector

} // END namespace containers


/* *********************************************************************** */
// #define _vector structures::containers::vector
template <typename T> using _vector = structures::vector<T>;


/* *********************************************************************** */
/* *********************************************************************** */

/// Constructors
template <typename T>
structures::vector<T>::vector() : dd(0), rr(0), avmem(0), rowMatrix(false) {}

template <typename T>
structures::vector<T>::vector(unsigned int rows) : rowMatrix(false) {
    rr= rows;
    avmem= rows;
    dd= new T[avmem];
}

/// Copy constructor
template <typename T>
structures::vector<T>::vector(const vector<T>& v) : rowMatrix(false) {
    rr= v.rr;
    avmem= v.rr;
    dd= new T[avmem];
    for (unsigned int i=0; i<rr; i++) dd[i]= v.dd[i];
}

template <typename T>
structures::vector<T>::vector(const T *arr, unsigned int n) : rowMatrix(false) {
		avmem= rr= n;
		dd= new T[avmem];
		for (unsigned int i=0; i<n; i++) dd[i]= arr[i];
	}

/// Destructor
template <typename T>
structures::vector<T>::~vector() {
	if (!rowMatrix) {
		if (dd) {
			delete []dd;
		}
	}
	dd= 0;
	rr= avmem= 0;
}

/// Assignment operator
template <typename T>
structures::vector<T>& structures::vector<T>::operator=(const structures::vector<T>& v) {
    if (this!=&v) {
        resize(v.rr);
	for (unsigned int i=0; i<rr; i++) dd[i]= v.dd[i];
    }
    return *this;
}

/// Comparison operator
template <typename T>
bool structures::vector<T>::operator==(const structures::vector<T>& v) const {
    if (this!=&v) {
        if (rr!=v.rr) return false;
        for (unsigned int i=0; i<rr; i++) {
            if (dd[i]!=v.dd[i]) return false;
        }
    }
    return true;
}

/// Access to elements
template <typename T>
T& structures::vector<T>::operator[](unsigned int i) {
    if (i>=rr) resize(i+1); // Resize the vector and set an element

    return dd[i]; // Set an element
}

template <typename T>
T structures::vector<T>::operator[](unsigned int i) const {
    return dd[i];
}

/// Returns the subvector obtained by eliminating the (i+1)th row
template <typename T>
structures::vector<T> structures::vector<T>::without(unsigned int i) {
    if (i>=rr) return *this; // Returns the whole vector

    // Returns the subvector
    vector<T> aux(rr-1);
    unsigned int k, j;

    for (k=0,j=0; k<i; k++,j++) aux.dd[j]= dd[k];
    for (k=i+1; k<=aux.rows(); k++,j++) aux.dd[j]= dd[k];

    return aux;
}

/// Resizes vector
template <typename T>
void structures::vector<T>::resize(unsigned int rows) {
    if (rr==rows) return;
    if (rows==0) {
		rr= 0;
        return;
    }
    if (dd==0) {
	rr= rows;
	avmem= rows;
	dd= new T[rows];
	return;
    }
    if (rows>avmem) {
	rr= rows;
	avmem= rows;
	dd= reallocate(dd,rows);
	return;
    }
    rr= rows;
}

/// Appends rows to vector
template <typename T>
void structures::vector<T>::append(sint rows) {
    if (rows==0) return;
    resize(rr+rows);
}

/// Deletes the ith row
template <typename T>
void structures::vector<T>::pop(unsigned int i) {
    if (i<0 || i>=rr) return;

    for (unsigned int k=i+1; k<rr; k++) dd[k-1]= dd[k];
    rr--;
}

/// Finds the minimum
template <typename T>
T structures::vector<T>::min() const {
 T M= dd[0];
 for (unsigned int i=1; i<rr; i++)
  if (M>dd[i]) M= dd[i];
 return M;
}

template <typename T>
T structures::vector<T>::min(const structures::vector<unsigned int> &indices) const {
 T M= dd[indices[0]];
 for (unsigned int i=1; i<indices.rows(); i++)
  if (M>dd[indices[i]]) M= dd[indices[i]];
 return M;
}

/// Finds the maximum
template <typename T>
T structures::vector<T>::max() const {
 T M= dd[0];
 for (unsigned int i=1; i<rr; i++)
  if (M<dd[i]) M= dd[i];
 return M;
}

template <typename T>
T structures::vector<T>::max(const structures::vector<unsigned int> &indices) const {
 T M= dd[indices[0]];
 for (unsigned int i=1; i<indices.rows(); i++)
  if (M<dd[indices[i]]) M= dd[indices[i]];
 return M;
}

/// Sums all the elements
template <typename T>
T structures::vector<T>::sum() const {
 T S= T(0);
 for (unsigned int i=0; i<rr; i++)
     S= S + dd[i];
 return S;
}

template <typename T>
T structures::vector<T>::sum(const structures::vector<unsigned int> &indices) const {
 T S= T(0);
 for (unsigned int i=0; i<indices.rows(); i++)
     S= S + dd[indices[i]];
 return S;
}

/// Sums all the elements
template <typename T>
T structures::vector<T>::sum2() const {
 T S= T(0);
 for (unsigned int i=0; i<rr; i++)
     S= S + dd[i]*dd[i];
 return S;
}

template <typename T>
T structures::vector<T>::sum2(const structures::vector<unsigned int> &indices) const {
 T S= T(0);
 for (unsigned int i=0; i<indices.rows(); i++)
     S= S + dd[indices[i]]*dd[indices[i]];
 return S;
}

/// Sorts the vector
template <typename T>
void structures::vector<T>::sort() {
    shellsort(dd,rr);
}

/// Shell sort
template <typename T>
void structures::vector<T>::shellsort(T *w, unsigned int n) {
    unsigned int i, j, inc;
    T v;
    inc= 1;
    do {
	inc*= 3;
	inc++;
    } while (inc<=n);
    do {
	inc/= 3;
	for (i=inc; i<n; i++) {
	    v= w[i];
	    j= i;
	    while (j>=inc && w[j-inc]>v) {
		w[j]= w[j-inc];
		j-= inc;
	    }
	    w[j]= v;
	}
    } while (inc>1);
}

/// I/O functions
template <typename T>
char structures::vector<T>::eoe[]={" "};

/// Overloaded operators for I/O
template <typename T>
std::ostream& structures::operator<<(std::ostream &os, const structures::vector<T> &v) {
    if (v.rr==0) return os;
    unsigned int i;
    for (i=0; i<(v.rr-1); i++) os<<v.dd[i]<<v.eoe;
    os<<v.dd[i];
    return os;
}

template <typename T>
std::istream& structures::operator>>(std::istream &is, structures::vector<T> &v) {
	std::string line;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    std::getline(is, line);
    std::istringstream stream(line);
	unsigned int n=0;
	T vj;
    while (stream>>vj) {
		n++;
	}
	v.resize(n);

	stream.clear();
	stream.str(line);
	n=0;
	while (stream>>vj) {
		v.dd[n++]= vj;
	}

	return is;
}

template <typename T>
void structures::vector<T>::saveBinary(const char* name) {
    std::ofstream f(name,std::ofstream::binary);
    f.write((char*)&rr,sizeof(unsigned int));
	if (rr!=0) f.write((char*)dd,sizeof(T)*rr);
    f.close();
}

template <typename T>
void structures::vector<T>::loadBinary(const char* name) {
    std::ifstream f(name,std::ifstream::binary);
    unsigned int r;
    f.read((char*)&r,sizeof(unsigned int));
    resize(r);
    f.read((char*)dd,sizeof(T)*r);
    f.close();
}

template <typename T>
void structures::vector<T>::saveText(const char* name) {
    std::ofstream f(name);
	if (rr!=0) {
		unsigned int i;
		for (i=0; i<(rr-1); i++) f<<dd[i]<<"\n";
		f<<dd[i]<<"\n";
	}
	f.close();
}

template <typename T>
void structures::vector<T>::loadText(const char* name) {
    std::ifstream f(name, std::ifstream::in);
	std::string ll;
	unsigned int i=0;

	while (f.good()) {
		std::getline(f,ll);
		if (!ll.size()) {
			break;
		}
		i++;
	}

	if (i==0) {
		f.close();
		return;
	}

	resize(i);

	f.clear();
	f.seekg(0, std::ifstream::beg);
	for (i=0; i<rr; i++) {
		f>>dd[i];
	}

	f.close();
}

template <typename T>
void structures::vector<T>::copyArray(const T* arr, unsigned int n) {
	resize(n);
	for (unsigned int i=0; i<n; i++) dd[i]= arr[n];
}

#endif // my_VECTOR_HPP
