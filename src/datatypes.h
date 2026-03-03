/*! @file datatypes.h
 *  Some synonyms/typedefs
 */

#ifndef DATA_TYPES_H
#define DATA_TYPES_H

//! Custom types
typedef long int sint;		///< signed integers
typedef double real;        ///< floating point numbers or reals

#include <string>

#ifdef USING_GC
	#include <gc/gc_allocator.h>
	typedef std::basic_string< char,std::char_traits<char>, gc_allocator<char> > text;	///< strings
#else
	typedef std::string text;	///< strings
#endif

#endif // DATA_TYPES_H
