#include "Memory.h"

void* operator new(const size_t Size) { return memory::AllocPaged(Size, 0); }

void* operator new(const size_t, void* Ptr) { return Ptr; }

void operator delete(void* Ptr, size_t) { memory::Free(Ptr, 0); }
