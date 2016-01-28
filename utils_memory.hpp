/**
  * helper functions to abstrct the backend
  *
  */

#ifndef UTILS_MEMORY_HPP
#define UTILS_MEMORY_HPP

#include <algorithm>

#include "utils_types.hpp"

namespace W2E
{

namespace Utils
{

// http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogObvious
cSize roundToAligned(const cSize& d)
{
  cSize v = d;
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return v;
}

// based on:
// https://github.com/CppCon/CppCon2015/tree/master/Presentations/allocator%20Is%20to%20Allocation%20what%20vector%20Is%20to%20Vexation

// memory block
struct Blk
{
  void* ptr;
  cSize size;
};

///////////////////////////////////////////////////////////////////////////////
// Fallback: try primary, if fails call secondary
///////////////////////////////////////////////////////////////////////////////

template < class Primary, class Fallback >
class FallbackAllocator : private Primary, private Fallback
{
public:
  Blk allocate(cSize n);
  void deallocate(Blk b);
  bool owns(Blk b);
};

template < class Primary, class Fallback >
Blk FallbackAllocator< Primary, Fallback >::allocate(cSize n)
{
  Blk r = Primary::allocate(n);
  if(!r.ptr)
  {
    r = Fallback::allocate(n);
  }
  return r;
}

template < class Primary, class Fallback >
void FallbackAllocator< Primary, Fallback >::deallocate(Blk b)
{
  if(Primary::owns(b))
  {
    Primary::deallocate(b);
  }
  else
  {
    Fallback::deallocate(b);
  }
}

template < class Primary, class Fallback >
bool FallbackAllocator< Primary, Fallback >::owns(Blk b)
{
  return Primary::owns(b) || Fallback::owns(b);
}

///////////////////////////////////////////////////////////////////////////////
// Stack: Use static array and static semantics
///////////////////////////////////////////////////////////////////////////////

template < cSize s >
class StackAllocator
{
public:
  StackAllocator()
      : data_{}
      , pointer_{data_}
  {
  }
  Blk allocate(cSize n);
  void deallocate(Blk b);
  bool owns(Blk b);

private:
  char data_[s];
  char* pointer_;
};

template < cSize s >
Blk StackAllocator< s >::allocate(cSize n)
{
  auto n1 = roundToAligned(n);
  if(n1 > (data_ + s) - pointer_)
  {
    return {nullptr, 0};
  }
  Blk result = {pointer_, n};
  pointer_ += n1;
  return result;
}

template < cSize s >
void StackAllocator< s >::deallocate(Blk b)
{
  // FIXME: arithmetic with void pointers have undefined behaviour
  if(b.ptr + roundToAligned(b.size) == pointer_)
  {
    pointer_ = b.ptr;
  }
}

template < cSize s >
bool StackAllocator< s >::owns(Blk b)
{
  return b.ptr >= data_ && b.ptr < data_ + s;
}

///////////////////////////////////////////////////////////////////////////////
// Stack: Keeps list of previous allocations of a given size
///////////////////////////////////////////////////////////////////////////////

template < class Parent, cSize minSize, cSize maxSize, cSize maxNum >
class Freelist
{
public:
  explicit Freelist(Parent p)
      : root_{nullptr}
      , parent_{p}
  {
  }

  Blk allocate(cSize n);
  void deallocate(Blk b);
  bool owns(Blk b);

private:
  Freelist() = delete;
  struct Node
  {
    Node* next;
  } root_;
  Parent parent_;
  cSize countDown_{maxNum};
};

template < class Parent, cSize minSize, cSize maxSize, cSize maxNum >
Blk Freelist< Parent, minSize, maxSize, maxNum >::allocate(cSize n)
{
  if(n >= minSize && n < maxSize && root_ && countDown_ < maxNum)
  {
    Blk b = {root_, n};
    root_ = root_.next;
    ++countDown_;
    return b;
  }
  return parent_.allocate(n);
}

template < class Parent, cSize minSize, cSize maxSize, cSize maxNum >
void Freelist< Parent, minSize, maxSize, maxNum >::deallocate(Blk b)
{
  if(b.size < minSize || b.size > maxSize || countDown_ == 0)
  {
    parent_.deallocate(b);
  }
  else
  {
    auto p = static_cast< Node* >(b.ptr);
    p.next = root_;
    root_ = p;
    --countDown_;
  }
}

template < class Parent, cSize minSize, cSize maxSize, cSize maxNum >
bool Freelist< Parent, minSize, maxSize, maxNum >::owns(Blk b)
{
  return (b.size >= minSize && b.size < maxSize) || parent_.owns(b);
}

// TODO:
// //------------------------------------------
// template <class A, size_t blockSize>
// class BitmappedBlock;
// //------------------------------------------
// template <class Creator>
// class CascadingAllocator;
// ...
// auto a = cascadingAllocator({
// return Heap<...>();
// });
// //------------------------------------------
// template <size_t threshold,
// class SmallAllocator,
// class LargeAllocator>
// struct Segregator;
// //------------------------------------------
// template <class Allocator,
// size_t min,
// size_t max,
// size_t step>
// struct Bucketizer;
// • [min, min + step),
// [min + step, min + 2 * step). . .
// • Within a bucket allocates the maximum size
// //------------------------------------------
// Complete API
// static constexpr unsigned alignment;
// static constexpr goodSize(size_t);
// Blk allocate(size_t);
// Blk allocateAll();
// bool expand(Blk&, size_t delta);
// void reallocate(Blk&, size_t);
// bool owns(Blk);
// void deallocate(Blk);
// void deallocateAll();

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_MEMORY_HPP
