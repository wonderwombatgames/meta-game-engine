/**
  * helper functions to abstrct the backend
  *
  */

#ifndef UTILS_FIX_CONTAINERS_HPP
#define UTILS_FIX_CONTAINERS_HPP

#include "utils/containers.hpp"

namespace W2E
{

namespace Utils
{

///////////////////////////////////////////////////////////////////////////////
// FixedArray : parametric base for array, stack, and deq
///////////////////////////////////////////////////////////////////////////////

template < typename Type, cSize Capacity >
struct FixedArray : public ArrayInterface< Type >
{
  using ElementType = Type;
  GLOBAL const cSize capacity_;

  cSize initialLen_;
  Type buffer_[Capacity];

  virtual cSize capacity() override { return capacity_; };
  virtual cSize initialLen() override { return initialLen_; };

  FixedArray() = delete;
  explicit FixedArray(const Type& init);
  FixedArray(const FixedArray& other);
  FixedArray& operator=(const FixedArray& other);
  // TODO: Move constructor???
  virtual ~FixedArray() {}
};

// GLOBAL
template < typename Type, cSize Capacity >
const cSize FixedArray< Type, Capacity >::capacity_{Capacity};

// implements constructors
template < typename Type, cSize Capacity >
FixedArray< Type, Capacity >::FixedArray(const Type& init)
    : ArrayInterface< Type >()
    , initialLen_{Capacity}
    , buffer_{}
{
  // ArrayInterface initializes firstPos_=0, lastPos_=0, length_=0, array_=nullptr
  this->lastPos_ = Capacity;
  this->length_ = Capacity;
  this->array_ = this->buffer_;
  std::fill(this->array_, (this->array_ + this->capacity_), init);
}

// constructors
template < typename Type, cSize Capacity >
FixedArray< Type, Capacity >::FixedArray(const FixedArray& other)
    : ArrayInterface< Type >(other)
    , initialLen_{other.initialLen_}
    , buffer_{}
{
  // ArrayInterface copies firstPos_=other, lastPos_=other, length_=other, array_=nullptr
  this->array_ = this->buffer_;
  std::copy(other.array_, (other.array_ + this->capacity_), this->array_);
}

template < typename Type, cSize Capacity >
FixedArray< Type, Capacity >& FixedArray< Type, Capacity >::operator=(const FixedArray& other)
{
  std::copy(
      other.array_, (other.array_ + std::min(other.capacity_, this->capacity_)), this->array_);
  this->firstPos_ = other.firstPos_;
  this->lastPos_ = other.lastPot_;
  this->length_ = std::min(other.length_, this->length_);
  this->initialLen_ = std::min(other.initialLen_, this->initialLen_);
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
// DEQueue : Fixed size double ended queue with random accessor
///////////////////////////////////////////////////////////////////////////////

template < typename Type, cSize Capacity, bool canOverwrite = false >
struct FixedDEQ : public DEQInterface< Type >
{
  using ElementType = Type;
  GLOBAL const cSize capacity_;

  cSize initialLen_;
  Type buffer_[Capacity];

  virtual cSize capacity() override { return capacity_; };
  virtual cSize initialLen() override { return initialLen_; };

  FixedDEQ() = delete;
  explicit FixedDEQ(const Type& init);
  FixedDEQ(const FixedDEQ& other);
  FixedDEQ& operator=(const FixedDEQ& other);
  // TODO: Move constructor???
  virtual ~FixedDEQ() {}
};

// GLOBAL
template < typename Type, cSize Capacity, bool canOverwrite >
const cSize FixedDEQ< Type, Capacity, canOverwrite >::capacity_{Capacity};

// implements constructors
template < typename Type, cSize Capacity, bool canOverwrite >
FixedDEQ< Type, Capacity, canOverwrite >::FixedDEQ(const Type& init)
    : DEQInterface< Type >()
    , initialLen_{0}
    , buffer_{}
{
  // DEQInterface inits firstPos_=0, lastPos_=0, length_=0, array_=nullptr, canOverwrite_=false
  this->canOverwrite_ = canOverwrite;
  this->array_ = this->buffer_;
  std::fill(this->array_, (this->array_ + this->capacity_), init);
}

// constructors
template < typename Type, cSize Capacity, bool canOverwrite >
FixedDEQ< Type, Capacity, canOverwrite >::FixedDEQ(const FixedDEQ& other)
    : DEQInterface< Type >(other)
    , initialLen_{other.initialLen_}
    , buffer_{}
{
  // DEQInterface copies firstPos_=other, lastPos_=other, length_=other, array_=nullptr,
  // canOverwrite_=false
  this->canOveWrite_ = canOverwrite;
  this->array_ = this->buffer_;
  std::copy(other.array_, (other.array_ + this->capacity_), this->array_);
}

template < typename Type, cSize Capacity, bool canOverwrite >
FixedDEQ< Type, Capacity, canOverwrite >& FixedDEQ< Type, Capacity, canOverwrite >::
operator=(const FixedDEQ& other)
{
  std::copy(
      other.array_, (other.array_ + std::min(other.capacity_, this->capacity_)), this->array_);
  this->firstPos_ = other.firstPos_;
  this->lastPos_ = other.lastPot_;
  this->canOveWrite_ = other.canOverwrite_;
  this->length_ = std::min(other.length_, this->length_);
  this->initialLen_ = std::min(other.initialLen_, this->initialLen_);
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
// RingQ : fixed size, acessible back and front with overwriting
///////////////////////////////////////////////////////////////////////////////

template < typename Type, cSize Capacity >
using FixedRingQ = FixedDEQ< Type, Capacity, true >;

///////////////////////////////////////////////////////////////////////////////
// BitMap : Fixed size Array of bits with random accessor
///////////////////////////////////////////////////////////////////////////////

template < cSize Capacity >
using FixedBitMap = FixedArray< u8, (Capacity >> 3) >;

///////////////////////////////////////////////////////////////////////////////
// List : Fixed size double linked list
///////////////////////////////////////////////////////////////////////////////

template < typename Type, cSize Capacity >
struct FixedList : public ListInterface< Type >
{

  FixedList() = delete;
  explicit FixedList(const Type& init);
  FixedList(const FixedList& other);
  FixedList& operator=(const FixedList& other);
  // TODO: Move constructor???
  virtual ~FixedList() {}
};

///////////////////////////////////////////////////////////////////////////////
// Tree : Fixed size tree containter (red-black)
///////////////////////////////////////////////////////////////////////////////

// declarations
template < typename Type >
struct FixedTree : public TreeInterface
{
  FixedTree() = delete;
  explicit FixedTree(const Type& init);
  FixedTree(const FixedTree& other);
  FixedTree& operator=(const FixedTree& other);
  // TODO: Move constructor???
  virtual ~FixedTree() {}
};

///////////////////////////////////////////////////////////////////////////////
// FixedTrie : Fixed size trie containter
///////////////////////////////////////////////////////////////////////////////

// declarations
template < typename Type >
struct FixedTrie : public TrieInterface
{
  FixedTrie() = delete;
  explicit FixedTrie(const Type& init);
  FixedTrie(const FixedTrie& other);
  FixedTrie& operator=(const FixedTrie& other);
  // TODO: Move constructor???
  virtual ~FixedTrie() {}
};

///////////////////////////////////////////////////////////////////////////////
// Accessor fucntions
///////////////////////////////////////////////////////////////////////////////

// FixedBitMap:
template < cSize Capacity >
inline u8 byte(FixedBitMap< Capacity >& container, cSize pos)
{
  return at(container, pos);
}

template < cSize Capacity >
inline u8 bit(FixedBitMap< Capacity >& container, cSize pos)
{
  const u8 bitMask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
  cSize byte_ = pos >> 3;

  if(byte_ < container.length_)
  {
    cSize bit_ = pos % 8;
    return (bitMask[bit_] & byte(container, byte_));
  }
  return false;
}

template < cSize Capacity >
inline u8 bits(FixedBitMap< Capacity >& container, cSize pos)
{
  return (8 * container.length_);
}

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_FIX_CONTAINERS_HPP
