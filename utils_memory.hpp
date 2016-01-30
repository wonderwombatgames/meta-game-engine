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
// Segregator: Sizes ≤ threshold goes to SmallAllocator, else goes to LargeAllocator
///////////////////////////////////////////////////////////////////////////////
template < cSize threshold, class SmallAllocator, class LargeAllocator >
class Segregator
{
public:
  Blk allocate(cSize n);
  void deallocate(Blk b);
  bool owns(Blk b);
};

template < cSize threshold, class SmallAllocator, class LargeAllocator >
Blk Segregator< threshold, SmallAllocator, LargeAllocator >::allocate(cSize n)
{
  Blk r;
  if(threshold > n)
  {
    r = SmallAllocator::allocate(n);
  }
  r = LargeAllocator::allocate(n);
  return r;
}

template < cSize threshold, class SmallAllocator, class LargeAllocator >
void Segregator< threshold, SmallAllocator, LargeAllocator >::deallocate(Blk b)
{
  if(SmallAllocator::owns(b))
  {
    SmallAllocator::deallocate(b);
  }
  else
  {
    LargeAllocator::deallocate(b);
  }
}

template < cSize threshold, class SmallAllocator, class LargeAllocator >
bool Segregator< threshold, SmallAllocator, LargeAllocator >::owns(Blk b)
{
  return SmallAllocator::owns(b) || LargeAllocator::owns(b);
}

///////////////////////////////////////////////////////////////////////////////
// Slicer: sits on top of bulk blocks allocator and provide just a slice
///////////////////////////////////////////////////////////////////////////////
template < class BulkAllocator, cSize size, cSize maxSlices = 64 >
class Slicer
{
public:
  explicit Slicer(BulkAllocator& bulkAlloc)
      : sliceMap_{0}
      , data_{nullptr}
      , bulkAlloc_{bulkAlloc}
  {
  }

  Blk allocate(cSize n);
  void deallocate(Blk b);
  bool owns(Blk b);

private:
  Slicer() = delete;
  Slicer(Slicer& other) = delete;
  Slicer& operator=(Slicer& other) = delete;

  u8 sliceMap_[maxSlices >> 3];
  char* data_;
  BulkAllocator& bulkAlloc_;
};

template < class BulkAllocator, cSize size, cSize maxSlices >
Blk Slicer< BulkAllocator, size, maxSlices >::allocate(cSize n)
{
  // TODO: checks if there is data_ (!= nullptr)
  // then return one of the free slices
  return {nullptr, 0};
}

template < class BulkAllocator, cSize size, cSize maxSlices >
void Slicer< BulkAllocator, size, maxSlices >::deallocate(Blk b)
{
}

template < class BulkAllocator, cSize size, cSize maxSlices >
bool Slicer< BulkAllocator, size, maxSlices >::owns(Blk b)
{
  return this->data_ != nullptr && b.ptr >= this->data_ && b.ptr < this->data_ + (size * maxSlices);
}

///////////////////////////////////////////////////////////////////////////////
// Freelist: Keeps list of previous allocations of a given size
///////////////////////////////////////////////////////////////////////////////

template < class Parent, cSize minSize, cSize maxSize, cSize maxBlocks >
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
  Freelist(Freelist& other) = delete;
  Freelist& operator=(Freelist& other) = delete;

  struct Node
  {
    Node* next;
  } root_;
  Parent parent_;
  cSize countDown_{maxBlocks};
};

template < class Parent, cSize minSize, cSize maxSize, cSize maxBlocks >
Blk Freelist< Parent, minSize, maxSize, maxBlocks >::allocate(cSize n)
{
  if(n >= minSize && n < maxSize && root_ && countDown_ < maxBlocks)
  {
    Blk b = {root_, n};
    root_ = root_.next;
    ++countDown_;
    return b;
  }
  return parent_.allocate(n);
}

template < class Parent, cSize minSize, cSize maxSize, cSize maxBlocks >
void Freelist< Parent, minSize, maxSize, maxBlocks >::deallocate(Blk b)
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

template < class Parent, cSize minSize, cSize maxSize, cSize maxBlocks >
bool Freelist< Parent, minSize, maxSize, maxBlocks >::owns(Blk b)
{
  return (b.size >= minSize && b.size < maxSize) || parent_.owns(b);
}

///////////////////////////////////////////////////////////////////////////////
// MAllocator: simple wraper around malloc to keep the interface consistent
///////////////////////////////////////////////////////////////////////////////

class MAllocator
{
public:
  Blk allocate(cSize n);
  void deallocate(Blk b);
  bool owns(Blk b);

private:
  MAllocator(MAllocator& other) = delete;
  MAllocator& operator=(const MAllocator& other) = delete;
};

Blk MAllocator::allocate(cSize n)
{
  Blk r;
  r.ptr = std::malloc(n * sizeof(char));
  r.size = n;

  return r;
}
void MAllocator::deallocate(Blk b) { std::free(b.ptr); }
bool MAllocator::owns(Blk b)
{
  // no way to control this
  // other allocators should use this as fall back or provider
  return false;
}

///////////////////////////////////////////////////////////////////////////////
// Stack: Use static array and stack semantics
///////////////////////////////////////////////////////////////////////////////

template < cSize size >
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
  StackAllocator(StackAllocator& other) = delete;
  StackAllocator& operator=(const StackAllocator& other) = delete;

  char data_[size];
  char* pointer_;
};

template < cSize size >
Blk StackAllocator< size >::allocate(cSize n)
{
  auto nn = roundToAligned(n);
  if(nn > (data_ + size) - pointer_)
  {
    return {nullptr, 0};
  }
  Blk result = {pointer_, n};
  pointer_ += nn;
  return result;
}

template < cSize size >
void StackAllocator< size >::deallocate(Blk b)
{
  if(static_cast< char* >(b.ptr) + roundToAligned(b.size) == pointer_)
  {
    pointer_ = b.ptr;
  }
}

template < cSize size >
bool StackAllocator< size >::owns(Blk b)
{
  return b.ptr >= this->data_ && b.ptr < this->data_ + size;
}

///////////////////////////////////////////////////////////////////////////////
// PoolAllocator: uses malloc to get a big chunck and manages it
// in a memory pool using a bitmap
///////////////////////////////////////////////////////////////////////////////

template < cSize size, cSize block >
class PoolAllocator
{
public:
  PoolAllocator()
      : data_{nullptr}
      , pointer_{nullptr}
      , freeChuncks_{0}
      , map_{}
  {
    this->freeChuncks_ = static_cast< u32 >(size / block);
    this->data_ = static_cast< char* >(std::calloc(size, sizeof(char)));
    this->pointer = this->data_;
    std::fill(this->map, this->map + this->freeChuncks_, 0);
  }
  ~PoolAllocator()
  {
    assert(this->freeChuncks_ == static_cast< u32 >(size / block));
    std::free(static_cast< void* >(this->data_));
  }

  Blk allocate(cSize n);
  void deallocate(Blk b);
  bool owns(Blk b);

private:
  PoolAllocator(PoolAllocator& other) = delete;
  PoolAllocator& operator=(const PoolAllocator& other) = delete;

  char* data_;
  char* pointer_;
  u32 freeChuncks_;
  u8 map_[static_cast< u32 >(size / block) >> 3];

  const u8 bitMask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
};

template < cSize size, cSize block >
Blk PoolAllocator< size, block >::allocate(cSize n)
{
  cSize nn = roundToAligned(n);
  if(nn == block && this->freeChuncks_)
  {
    Blk r;
    // free block at the end of the chunk
    if(this->pointer_ < (this->data_ + size))
    {
      r.ptr = static_cast< void* >(this->pointer_);
      r.size = n;
      u32 offset = static_cast< u32 >((this->pointer_ - this->data_) / block);
      u8 bitset = offset % 8;
      offset = offset >> 3;
      this->map_[offset] |= bitMask[bitset];
      this->pointer_ += nn;
      --(this->freeChuncks_);
      return r;
    }
    else
    // needs to find a free block
    {
      int offset = 0;
      for(; offset < (static_cast< u32 >(size / block) >> 3); ++offset)
      {
        u8 bmap = this->map_[offset];
        if(bmap < 0xff)
        {
          u8 rev = ~bmap;
          u8 bitset = 0x07;
          while(bitset <= 0x00)
          {
            u8 mask = bitMask[bitset];
            if(mask & rev)
            {
              u32 delta = (offset << 3);
              delta += bitset;
              r.ptr = static_cast< void* >(this->data_ + delta);
              r.size = n;
              this->pointer_ += nn;
              this->map_[offset] |= mask;
              --(this->freeChuncks_);
              return r;
            }
            --bitset;
          }
        }
      }
    }
  }
  return {nullptr, 0};
}

template < cSize size, cSize block >
void PoolAllocator< size, block >::deallocate(Blk b)
{
  u32 offset = static_cast< u32 >((static_cast< char* >(b.ptr) - this->data_) / block);
  u8 bitset = offset % 8;
  offset = offset >> 3;
  this->map_[offset] &= (~bitMask[bitset]);
}

template < cSize size, cSize block >
bool PoolAllocator< size, block >::owns(Blk b)
{
  return static_cast< char* >(b.ptr) >= data_ && static_cast< char* >(b.ptr) < this->data_ + size;
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
