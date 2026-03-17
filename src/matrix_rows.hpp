/*! @file matrix_rows.hpp
 *  Template class to use matrix like structure; used mainly to store data
 */

#ifndef my_MATRIX_ROWS_HPP
#define my_MATRIX_ROWS_HPP

#include "memalloc.hpp"
#include "datatypes.h"
#include "vector.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

namespace structures {

///-----------------------------------------------------------------------------
/// Matrix class; declaration
///-----------------------------------------------------------------------------

/*! @class matrix<T>
 *  This template class creates a matrix structure that stores data of type/class T
 *
 * It wouldn't be used as math matrix because arithmetic and logic operations
 * are not implemented
 */
template <typename T>
class matrix {

protected:

    //! @brief Matrix's data structure
    T *dd;      ///< pointer which stores the data
    unsigned int rr,    ///< number of rows
         cc,    ///< number of columns
    avmem;      ///< maximum number of elements available in dd

public:

    /// Constructors

    //! @brief Default constructor
    matrix();

    //! @brief Another constructor
    //! @param rows [in] number of rows
    //! @param cols [in] number of columns
    matrix(unsigned int rows, unsigned int cols);

    //! @brief Copy constructor
    //! @param m [in] reference matrix
    matrix(const matrix& m);

	matrix(const T* arr, unsigned int rows, unsigned int cols, bool byrow=true);

    /// @brief Destructor
    ~matrix();

    //! @brief Returns the number of rows of the matrix
    inline unsigned int rows() const { return rr; }

    //! @brief Returns the number of rows of the matrix
    inline unsigned int size() const { return rr; }

    //! @brief Returns the number of columns of the matrix
    inline unsigned int cols() const { return cc; }

    //! @brief Assignment operator
    //! @param m [in] matrix with the new contents
    matrix<T>& operator=(const matrix<T>& m);

    //! @brief Comparison operators
    //! @param m [in] comparing matrix
    bool operator==(const matrix<T>& m) const;
    bool inline operator!=(const matrix<T>& m) const { return !((*this)==m); }

    //! @brief Access to elements
    //! @param i [in] at row i
    //! @param j [in] and at column j
    //! if i+1>row, the matrix will be resized, setting the rows equals to i+1
    //! this doesn't work with j
    //! Usage: my_matrix(i,j)= entry_ij;
    T& operator()(unsigned int i, unsigned int j);
    T operator()(unsigned int i, unsigned int j) const;

    /// Access to rows

    //! @brief Returns the ith row
    //! @param i [in] row to be returned
    //! Usage: ith_row_vector= my_matrix[i];
    vector<T> operator[](unsigned int i) const;
	vector<T> operator[](unsigned int i);

    //! @brief Modifies row's contents taking  vector's data
    //! @param i [in] row to be modified
    //! @param v [in] vector whose data will be copied to the row
    //! Usage: my_matrix(i,new_ith_row_vector);
    void operator()(unsigned int i, const vector<T>& v);

    //! @brief Returns the submatrix obtained by eliminating the ith row
    //! @param i [in] row to be eliminated
    matrix<T> without(unsigned int i);

    //! @brief Resizes the matrix
    //! @param rows [in] new number of rows
	void resize(unsigned int rows, unsigned int cols) {
		setCols(cols);
		resize(rows);
	}

    void resize(unsigned int rows);

    //! @brief Appends rows at the end of the matrix
    //! @param rows [in] number of rows to be appended; if rows<0 the last -rows will be deleted
    void append(sint n);

    //! @brief Deletes the ith row
    //! @param i [in] index of the row to be deleted
    void pop(sint i);

    //! @brief Functions which add or delete rows
    //! @param n [in] number of rows to be added/deleted
    void addRows(unsigned int n);
    void delRows(unsigned int n);

    //! @brief Resizes the matrix, modifying only the number of cols
    //! @param cols [in] new number of columns
    void setCols(unsigned int cols);

    static char eoe[5],	///< text which indicates the en of each element
                eol[5];	///< text which indicates the en of each row

    //! @brief Overloaded operators for I/O
    template <typename t>
    friend std::ostream& operator<<(std::ostream &os, const matrix<t> &m);

/*   template <class t>
   friend std::ostream& operator<<(std::ostream &os, const matrix<t> &m) {
	if (m.rr==0) return os;
	unsigned int i,j;
	for (i=0; i<(m.rr-1); i++) {
	    for (j=0; j<(m.cc-1); j++) os<<m.dd[i+j*m.rr]<<m.eoe;
	    os<<m.dd[i+j*m.rr]<<m.eol;
	}
	for (j=0; j<(m.cc-1); j++) os<<m.dd[i+j*m.rr]<<m.eoe;
	    os<<m.dd[i+j*m.rr];
	return os;
    }
*/
    void saveBinary(const char* name);
    void loadBinary(const char* name);

    void saveText(const char* name);
    void loadText(const char* name);

	void inline save(const char* name, bool binary=true) {
		if (binary) saveBinary(name);
		else saveText(name);
	}

	void inline load(const char* name, bool binary=true) {
		if (binary) loadBinary(name);
		else loadText(name);
	}

	void copyArray(const T* arr, unsigned int rows, unsigned int cols, bool byrow=true);

}; // END class matrix

} // END namespace structures
/* *********************************************************************** */

// #define _matrix structures::matrix
template <typename T> using _matrix = structures::matrix<T>;

/* *********************************************************************** */
/* *********************************************************************** */


/// Constructors
template <typename T>
structures::matrix<T>::matrix() : dd(0), rr(0), cc(0), avmem(0) {}

template <typename T>
structures::matrix<T>::matrix(unsigned rows, unsigned cols) {
    cc= cols;
    rr= rows;
    avmem= cc*rr;
    dd= new T[avmem];
}

/// Copy constructor
template <typename T>
structures::matrix<T>::matrix(const matrix<T>& m) {
    cc= m.cc;
    rr= m.rr;
    avmem= cc*rr;
    dd= new T[avmem];
    for (unsigned int i=0; i<rr*cc; i++) dd[i]= m.dd[i];
}

template <typename T>
structures::matrix<T>::matrix(const T* arr, unsigned int rows, unsigned int cols, bool byrow) {
	cc= cols;
	rr= rows;
	avmem= cc*rr;
	dd= new T[avmem];
	copyArray(arr,rows,cols,byrow);
}

/// Destructor
template <typename T>
structures::matrix<T>::~matrix() {
    delete []dd, dd=0;
}

/// Assignment operator
template <typename T>
structures::matrix<T>& structures::matrix<T>::operator=(const structures::matrix<T>& m) {
    if (this!=&m) {
	if (avmem==0) {
		dd= new T[m.rr*m.cc];
	    avmem= m.rr*m.cc;
	}
	if (avmem<m.rr*m.cc) {
		dd= reallocate(dd,m.rr*m.cc);
	    avmem= m.rr*m.cc;
	}
        rr= m.rr;
        cc= m.cc;
	for (unsigned int i=0; i<rr*cc; i++) dd[i]= m.dd[i];
    }
    return *this;
}

/// Comparison operator
template <typename T>
bool structures::matrix<T>::operator==(const structures::matrix<T>& m) const {
    if (this!=&m) {
        if (rr!=m.rr || cc!=m.cc) return false;
        for (unsigned int i=0; i<rr*cc; i++) {
            if (dd[i]!=m.dd[i]) return false;
        }
    }
    return true;
}

/// Access to elements
template <typename T>
T& structures::matrix<T>::operator()(unsigned int i, unsigned int j) {
    if (i>=rr) resize(i+1);	// first, resizes the row's matrix if required

    return dd[i*cc+j];		// then, sets the element
}

/// Returns the (i,j) element
template <typename T>
T structures::matrix<T>::operator()(unsigned int i, unsigned int j) const {
    return dd[i*cc+j];
}

/// Returns the ith row
template <typename T>
structures::vector<T> structures::matrix<T>::operator[](unsigned int i) const {
	return vector<T>(dd+i*cc,cc);
}

template <typename T>
structures::vector<T> structures::matrix<T>::operator[](unsigned int i) {
	return vector<T>(dd+i*cc,cc);
}

/// Modifies the ith row
template <typename T>
void structures::matrix<T>::operator()(unsigned int i, const structures::vector<T>& v) {
    if (i>=rr) resize(i+1);

    for (unsigned int k=0; k<cc; k++) dd[i*cc+k]= v.dd[k];
}

/// Returns the submatrix obtained by eliminating the ith row
template <typename T>
structures::matrix<T> structures::matrix<T>::without(unsigned int i) {
    if (i>=rr) return *this; // if i is greater than the current number of rows, the whole matrix is returned

    // otherwise, the submatrix is created and filled
    unsigned int k, j;
    matrix<T> sm(rr-1,cc);
	for (k=0; k<cc*i; k++) sm.dd[k]= dd[k];
	for (; k<sm.rr*cc; k++) sm.dd[k]= dd[k+cc];
    return sm;
}

/// Resizes the matrix, modifying only the rows
template<typename T>
void structures::matrix<T>::resize(unsigned int rows) {
    if (rr==rows) return;
    if (rr>rows) delRows(rr-rows);
    else addRows(rows-rr);
}

/// Appends rows at the end of the matrix
template<typename T>
void structures::matrix<T>::append(sint rows) {
    if (rows==0) return;
    resize(rr+rows);
}

/// Deletes the ith row
template<typename T>
void structures::matrix<T>::pop(sint i) {
    if (i<0 || i>=rr) return;
    rr--;
	for (unsigned int k=cc*i; k<rr*cc; k++) dd[k]= dd[k+cc];
}

/// Adds rows to the matrix
template<typename T>
void structures::matrix<T>::addRows(unsigned int n) {
    if (n==0) return;

    if (cc==0) {
        rr+= n;
        return;
    }

    if(avmem==0) {
		dd= new T[(rr+n)*cc];
	    avmem= (rr+n)*cc;
	}
    if (avmem<(rr+n)*cc) {
	dd= reallocate(dd,(rr+n)*cc);
	avmem= (rr+n)*cc;
    }
    rr+= n;
}

/// Deletes rows to the matrix
template<typename T>
void structures::matrix<T>::delRows(unsigned int n) {
    if (n==0) return;

    if (cc==0) {
        if (n>=rr) rr=0;
        else rr-= n;
        return;
    }

    if (n>=rr) {
	rr= 0;
        return;
    }

    rr-= n;
}

/// Modifies column's number
template<typename T>
void structures::matrix<T>::setCols(unsigned int m) {
//     assert(m>=0);

    if (m==0) {
        cc= 0;
        return;
    }

    if (rr==0) {
        cc= m;
        return;
    }

    if (m==cc) return;

    if (cc==0) { // Did not exist allocated memory for dd
        dd= new T[rr*m];
		avmem=rr*m;
    }
    cc= m;
}

/// I/O functions
template <typename T>
char structures::matrix<T>::eoe[]={" "};

template <typename T>
char structures::matrix<T>::eol[]={"\n"};


/// Overloaded operators for I/O
template <typename T>
std::ostream& operator<<(std::ostream &os, const structures::matrix<T> &m) {
    if (m.rr==0) return os;
    unsigned int i,j;
    for (i=0; i<(m.rr-1); i++) {
	for (j=0; j<(m.cc-1); j++) os<<m.dd[i+j*m.rr]<<m.eoe;
	os<<m.dd[i+j*m.rr]<<m.eol;
    }
    for (j=0; j<(m.cc-1); j++) os<<m.dd[i+j*m.rr]<<m.eoe;
	os<<m.dd[i+j*m.rr];
    return os;
}

template <typename T>
void structures::matrix<T>::saveBinary(const char* name) {
    std::ofstream f(name,std::ofstream::binary);
    f.write((char*)&rr,sizeof(unsigned int));
    f.write((char*)&cc,sizeof(unsigned int));
	if ((rr*cc)!=0) f.write((char*)dd,sizeof(T)*rr*cc);
    f.close();
}

template <typename T>
void structures::matrix<T>::loadBinary(const char* name) {
    std::ifstream f(name,std::ifstream::binary);
    unsigned int r, c;
    f.read((char*)&r,sizeof(unsigned int));
    f.read((char*)&c,sizeof(unsigned int));
    setCols(c);
    resize(r);
	if ((r*c)!=0) f.read((char*)dd,sizeof(T)*r*c);
    f.close();
}

template <typename T>
void structures::matrix<T>::saveText(const char* name) {
	std::ofstream f(name);
	if (rr!=0) {
		unsigned int i,j;
		for (i=0; i<(rr-1); i++) {
			for (j=0; j<(cc-1); j++) f<<dd[i*cc+j]<<" ";
			f<<dd[i*cc+j]<<"\n";
		}
		for (j=0; j<(cc-1); j++) f<<dd[i*cc+j]<<" ";
		f<<dd[i*cc+j]<<"\n";
	}
	f.close();
}

template <typename T>
void structures::matrix<T>::loadText(const char* name) {
    std::ifstream f(name);
	std::string ll, tt;
	std::istringstream ss;

	unsigned int c=0, r=0;

	if (f.good()) {
		std::getline(f,ll);

		ss.str(ll);
		while (ss) {
			ss >> ll;
			c++;
		}
		r++;
		c--;
	} else {
		f.close();
		return;
	}

	while (f.good()) {
		std::getline(f,ll);
		if (!ll.size()) break;
		r++;
	}

	if (r==0) {
		f.close();
		return;
	}

	resize(r,c);

	f.clear();
	f.seekg(0, std::ifstream::beg);

	for (unsigned int i=0; i<r; i++) {
		std::getline(f,ll);
		ss.clear();
		ss.str(ll);
		for (unsigned int j=0; j<c; j++) {
			ss>> dd[i*c+j];
		}
	}

	f.close();
}

template <typename T>
void structures::matrix<T>::copyArray(const T* arr, unsigned int rows, unsigned int cols, bool byrow) {
	resize(rows,cols);

	if (byrow) {
		for (unsigned int i=0; i<rr*cc; i++) {
			dd[i]= arr[i];
		}
	} else {
		for (unsigned int i=0; i<rr; i++) {
			for (unsigned int j=0; j<cc; j++) {
				dd[i*cc+j]= arr[i+rr*j];
			}
		}
	}
}



#endif // my_MATRIX_ROWS_HPP
