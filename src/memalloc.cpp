#include "memalloc.hpp"

void* operator new(std::size_t size) {
  alloc++;
  return myMALLOC(size);
}

void operator delete(void* ptr) noexcept {
  dealloc++;
  myFREE(ptr);
}
