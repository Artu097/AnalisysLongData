#ifndef MEMALLOC_HPP
#define MEMALLOC_HPP

#ifdef USING_GC

	#include <gc/gc.h>

	#define myMALLOC GC_MALLOC
	#define myFREE GC_FREE
	#define myREALLOC GC_REALLOC

	#define COLLECT_GARBAGE GC_gcollect()
	#define INIT_GCOLLECT GC_INIT()

#else

	#define myMALLOC malloc
	#define myFREE free
	#define myREALLOC realloc

	#define COLLECT_GARBAGE
	#define INIT_GCOLLECT

#endif

#include <iostream>
static std::size_t alloc{0};
static std::size_t dealloc{0};

void getMemInfo();

/// Overriding new and delete operators

void *operator new(size_t size);

void operator delete(void* ptr);


/// Reallocating operations

template <typename T>
class ArrayRealloc {
public:
// handle differences between compiler implementations
#ifdef _MSC_VER
	typedef int			CountT;
#else
	typedef size_t		CountT;
#endif

public:
	ArrayRealloc(T *ptr);

	size_t count(void) const;

	void set(size_t newcnt);

	T *user(void) const;

	T *resize(size_t newcnt);

private:
	CountT *base;
};


class GenericObject {
public:
	template <typename T>
	static void construct(T *base, size_t count);

	template <typename T>
	static void destruct(T *base, size_t count);
};


template <typename T, bool has_constructor = __has_trivial_constructor(T), bool has_destructor = __has_trivial_destructor(T)>
class Reallocate;


template <typename T>
class Reallocate<T, true, true> {
public:
	static T *reallocate(T *ptr, size_t newcnt);
};


template <typename T>
class Reallocate<T, false, true> {
public:
	static T *reallocate(T *ptr, size_t newcnt);
};


template <typename T>
class Reallocate<T, true, false> {
public:
	static T *reallocate(T *ptr, size_t newcnt);
};


template <typename T>
class Reallocate<T, false, false> {
public:
	static T *reallocate(T *ptr, size_t newcnt);
};


template <typename T>
T* reallocate(T *ptr, size_t newcnt);


/* *********************************************************************** */
/* *********************************************************************** */

inline void getMemInfo() {
	std::cout << std::endl;

    std::cout << "Number of allocations: " << alloc << std::endl;
    std::cout << "Number of deallocations: " << dealloc << std::endl;

    std::cout << std::endl;
}

/// Overriding new and delete operators

inline void *operator new(size_t size) {
	return myMALLOC(size);
}

inline void operator delete(void* ptr) {
	myFREE(ptr);
}


/// Reallocating operations

template <typename T>
ArrayRealloc<T>::ArrayRealloc(T *ptr) {
	// calculate pointer to base of memory
	base = reinterpret_cast<CountT *>(ptr) - 1;
}

template <typename T>
size_t ArrayRealloc<T>::count(void) const {
	// return count of items in array
	return (size_t)*base;
}

template <typename T>
void ArrayRealloc<T>::set(size_t newcnt) {
	// change count of items in array
	*base = (CountT)newcnt;
}

template <typename T>
T* ArrayRealloc<T>::user(void) const {
	// return pointer to user memory
	return reinterpret_cast<T *>(base + 1);
}

template <typename T>
T* ArrayRealloc<T>::resize(size_t newcnt) {
	// reallocate the array to the specified size, plus the space needed for the array count
	void *newptr = myREALLOC(base, sizeof(CountT) + (newcnt * sizeof(T)));

	// update our base pointer as realloc might have moved the memory block
	base = static_cast<CountT *>(newptr);

	// update the array count, and return the pointer to reallocated user memory
	set(newcnt);
	return user();
}


template <typename T>
void GenericObject::construct(T *base, size_t count) {
	// create a pointer to the end of memory to begin construction
	T *end = base + count;

	// call the default constructor for each object using placement new, in increasing order
	while (base < end) {
		new (static_cast<void *>(base)) T();
		base++;
	}
}

template <typename T>
void GenericObject::destruct(T *base, size_t count) {
	// create a pointer to last object to destroy
	T *end = base + (count - 1);

	// call the destructor for each object directly, in reverse order
	while (end >= base) {
		end->~T();
		end--;
	}
}


template <typename T>
T* Reallocate<T, true, true>::reallocate(T *ptr, size_t newcnt) {
	// both trivial constructor and destructor, so just call realloc directly
	return static_cast<T *>(myREALLOC(ptr, newcnt * sizeof(T)));
}

template <typename T>
T* Reallocate<T, false, true>::reallocate(T *ptr, size_t newcnt) {
	// first determine current array count (no count is stored since the destructor is trivial)
#ifdef _MSC_VER
	size_t cnt = _msize(ptr) / sizeof(T);
#elif defined(__MACH__)
	size_t cnt = malloc_size(ptr) / sizeof(T);
#else
	size_t cnt = malloc_usable_size(ptr) / sizeof(T);
#endif

	// do nothing if reallocating to the same size
	if (cnt == newcnt) {
		return ptr;
	}

	// realloc to new size (there are no destructors to call)
	T *newptr = static_cast<T *>(myREALLOC(ptr, newcnt * sizeof(T)));

	// if we expanded, call constructors for those elements
	if (newcnt > cnt) {
		GenericObject::construct(newptr + cnt, newcnt - cnt);
	}

	return newptr;
}


template <typename T>
T* Reallocate<T, true, false>::reallocate(T *ptr, size_t newcnt) {
	ArrayRealloc<T> arr(ptr);

	// do nothing if reallocating to the same size
	if (arr.count() == newcnt) {
		return ptr;
	}

	// check if we are shrinking
	if (newcnt < arr.count()) {
		// destruct objects, then resize
		GenericObject::destruct(ptr + newcnt, arr.count() - newcnt);
		return arr.resize(newcnt);
	} else {
		// simply resize (no constructor to call)
		return arr.resize(newcnt);
	}
}


template <typename T>
T* Reallocate<T, false, false>::reallocate(T *ptr, size_t newcnt) {
	ArrayRealloc<T> arr(ptr);

	// do nothing if reallocating to the same size
	if (arr.count() == newcnt) {
		return ptr;
	}

	// check if we are shrinking
	if (newcnt < arr.count()) {
		// destruct objects, then resize
		GenericObject::destruct(ptr + newcnt, arr.count() - newcnt);
		return arr.resize(newcnt);
	} else {
		// resize first
		size_t oldcnt = arr.count();
		arr.resize(newcnt);

		// then construct the new objects
		GenericObject::construct(arr.user() + oldcnt, arr.count() - oldcnt);
		return arr.user();
	}
}

template <typename T>
T* reallocate(T *ptr, size_t newcnt) {
	return Reallocate<T>::reallocate(ptr, newcnt);
}

#endif // MEMALLOC_HPP
