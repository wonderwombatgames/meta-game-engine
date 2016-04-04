/**
  * implementation of containers using dynamic allocation
  * NOTE: an allocator has to be passe to the containers that
  * provides the allocation interface as in utils/memory.hpp
  *
  */

#ifndef UTILS_DYN_CONTAINERS_HPP
#define UTILS_DYN_CONTAINERS_HPP

#include <algorithm>

#include "utils/containers.hpp"
#include "utils/memory.hpp"

namespace W2E
{

namespace Utils
{

// forward declarations
struct Allocator;

///////////////////////////////////////////////////////////////////////////////
// Array : dynamic array with random accessor
///////////////////////////////////////////////////////////////////////////////

template < typename Type, typename Allocator >
struct Array : public ArrayInterface< Type >
{
  using ElementType = Type;
  using AllocatorType = Allocator;

  Allocator& alloc_;
  cSize capacity_;
  cSize initialLen_;
  Blk memBlock_;
  Array() = delete;

  // these are used by the base class
  virtual cSize capacity() override { return capacity_; };
  virtual cSize initialLen() override { return initialLen_; };

  // destructors and constructors
  Array(Allocator& alloc, const Type& init, const cSize Capacity);
  explicit Array(const Array& other);
  Array& operator=(const Array& other);
  // TODO: Move constructor???
  virtual ~Array();
};

// default virtual destructor
template < typename Type, typename Allocator >
Array< Type, Allocator >::~Array()
{
  if(this->memBlock_.ptr)
  {
    this->alloc_.deallocate(this->memBlock_);
  }
}

// constructor
template < typename Type, typename Allocator >
Array< Type, Allocator >::Array(Allocator& alloc, const Type& init, const cSize Capacity)
    : ArrayInterface< Type >()
    , alloc_{alloc}
    , capacity_{Capacity}
    , initialLen_{Capacity}
    , memBlock_{nullptr, 0}
{
  this->init_ = init;
  this->array_ = allocateType< Type, Allocator >(this->alloc_, this->memBlock_, this->capacity_);
  this->firstPos_ = 0;
  this->lastPos_ = Capacity;
  this->length_ = Capacity;

  std::fill(this->array_, (this->array_ + this->capacity_), this->init_);
}

// copy constructor
template < typename Type, typename Allocator >
Array< Type, Allocator >::Array(const Array& other)
    : ArrayInterface< Type >(other)
    , alloc_{other.alloc}
    , capacity_{other.capacity_}
    , initialLen_{other.capacity_}
    , memBlock_{nullptr, 0}
{
  this->firstPos_ = other.firstPos_;
  this->lastPos_ = other.capacity_;
  this->length_ = other.capacity_;
  this->init_ = other.init_;
  this->array_ = allocateType(this->alloc_, this->memBlock_, this->capacity_);
  std::copy(other.array_, (other.array_ + this->capacity_), this->array_);
}

// assignement operator
template < typename Type, typename Allocator >
Array< Type, Allocator >& Array< Type, Allocator >::operator=(const Array& other)
{
  if(this->memBlock_.ptr)
  {
    this->alloc_.deallocate(this->memBlock);
  }

  this->alloc_ = other.alloc;
  this->firstPos_ = other.firstPos_;
  this->lastPos_ = other.lastPos_;
  this->length_ = other.length_;
  this->capacity_ = other.capacity_;
  this->initialLen_ = other.capacity_;
  this->init_ = other.init_;
  this->memBlock_ = {nullptr, 0};

  this->array_ = allocateType(this->alloc_, this->memBlock_, this->capacity_);
  std::copy(other.array_, (other.array_ + this->capacity_), this->array_);

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
// DEQ : dynamic double ended queue with random accessor
///////////////////////////////////////////////////////////////////////////////

template < typename Type, typename Allocator, bool canOverwrite = false >
struct DEQ : public DEQInterface< Type >
{
  using ElementType = Type;
  using AllocatorType = Allocator;

  Allocator& alloc_;
  cSize capacity_;
  cSize initialLen_;
  Type init_;
  Blk memBlock_;
  DEQ() = delete;

  // these are used by the base class
  virtual cSize capacity() override { return capacity_; };
  virtual cSize initialLen() override { return initialLen_; };

  // destructors and constructors
  DEQ(Allocator& alloc, const Type& init, const cSize Capacity);
  explicit DEQ(const DEQ& other);
  DEQ& operator=(const DEQ& other);
  // TODO: Move constructor???
  virtual ~DEQ();
};

// virtual destructor
template < typename Type, typename Allocator, bool canOverwrite >
DEQ< Type, Allocator, canOverwrite >::~DEQ()
{
  if(this->memBlock_.ptr)
  {
    this->alloc_.deallocate(this->memBlock_);
  }
}

// constructor
template < typename Type, typename Allocator, bool canOverwrite >
DEQ< Type, Allocator, canOverwrite >::DEQ(Allocator& alloc, const Type& init, const cSize Capacity)
    : DEQInterface< Type >()
    , alloc_{alloc}
    , capacity_{Capacity}
    , initialLen_{0}
    , init_{init}
    , memBlock_{nullptr, 0}
{
  // DEQInterface inits firstPos_=0, lastPos_=0, length_=0, array_=nullptr, canOverwrite_=false
  this->canOverwrite_ = canOverwrite;
  this->array_ = allocateType< Type, Allocator >(this->alloc_, this->memBlock_, this->capacity_);

  std::fill(this->array_, (this->array_ + this->capacity_), this->init_);
}

// copy constructor
template < typename Type, typename Allocator, bool canOverwrite >
DEQ< Type, Allocator, canOverwrite >::DEQ(const DEQ& other)
    : DEQInterface< Type >(other)
    , alloc_{other.alloc}
    , capacity_{other.capacity_}
    , initialLen_{0}
    , init_{other.init_}
    , memBlock_{nullptr, 0}
{
  // DEQInterface copies firstPos_=other, lastPos_=other, length_=other, array_=nullptr,
  // canOverwrite_=false
  this->canOverwrite_ = other.canOverwrite_;
  this->array_ = allocateType(this->alloc_, this->memBlock_, this->capacity_);
  std::copy(other.array_, (other.array_ + this->capacity_), this->array_);
}

// assignement operator
template < typename Type, typename Allocator, bool canOverwrite >
DEQ< Type, Allocator, canOverwrite >& DEQ< Type, Allocator, canOverwrite >::
operator=(const DEQ& other)
{
  if(this->memBlock_.ptr)
  {
    this->alloc_.deallocate(this->memBlock);
  }

  this->alloc_ = other.alloc;
  this->firstPos_ = other.firstPos_;
  this->lastPos_ = other.lastPos_;
  this->length_ = other.length_;
  this->capacity_ = other.capacity_;
  this->initialLen_ = other.initialLen_;
  this->canOverwrite_ = other.canOverwrite_;
  this->init_ = other.init_;
  this->memBlock_ = {nullptr, 0};

  this->array_ = allocateType(this->alloc_, this->memBlock_, this->capacity_);
  std::copy(other.array_, (other.array_ + this->capacity_), this->array_);

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
// List : dynamic double linked list
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
template < typename Type, cSize Capacity >
struct List : public ListInterface< Type >
{
  List() = delete;
  explicit List(const Type& init);
  List(const List& other);
  List& operator=(const List& other);
  // TODO: Move constructor???
  virtual ~List() {}
};

///////////////////////////////////////////////////////////////////////////////
// Tree : dynamic balanced tree containter (red-black)
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
template < typename Type >
struct Tree : public TreeInterface
{
  Tree() = delete;
  Tree(Allocator& alloc, const Type& init);
  explicit Tree(const Tree& other);
  Tree& operator=(const Tree& other);
  // TODO: Move constructor???
  virtual ~Tree() {}
};

///////////////////////////////////////////////////////////////////////////////
// Trie : dynamic trie containter
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
// template < typename Type >
// struct Trie : public TrieInterface
// {
//   Trie() = delete;
//   Trie(Allocator& alloc, const Type& init);
//   explicit Trie(const Trie& other);
//   Trie& operator=(const Trie& other);
//   // TODO: Move constructor???
//   virtual ~Trie() {}
// };

///////////////////////////////////////////////////////////////////////////////
// HashMap : dynamic hash map containter
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
// template < typename Type, bool allowMultiple >
// struct HashMap : public HashMapInterface
// {
//   const bool allowMultiple_{allowMultiple};
//
//   HashMap() = delete;
//   explicit HashMap(Allocator& alloc, const Type& init);
//   explicit HashMap(const HashMap& other);
//   HashMap& operator=(const HashMap& other);
//   // TODO: Move constructor???
// };

///////////////////////////////////////////////////////////////////////////////
// Heap : dynamic heap containter
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
// template < typename Type >
// struct Heap : public HeapInterface
// {
//   Heap() = delete;
//   explicit Heap(Allocator& alloc, const Type& init);
//   explicit Heap(const Heap& other);
//   Heap& operator=(const Heap& other);
//   // TODO: Move constructor???
// };

// Resize: for linear containers
//-----------------------------------------------------------------
template < typename Container >
bool resize(Container& container, const cSize Capacity)
{
  using Type = typename Container::ElementType;
  using Allocator = typename Container::AllocatorType;

  if(Capacity > container.capacity_)
  {
    Blk newMemBlk;
    typename Container::ElementType* oldArray_ = container.array_;

    container.array_ = allocateType< Type, Allocator >(container.alloc_, newMemBlk, Capacity);
    std::fill((container.array_ + container.capacity_ - 1),
              (container.array_ + Capacity),
              container.init_);
    std::copy(oldArray_, (oldArray_ + container.capacity_), container.array_);
    container.length_ = Capacity;
    container.capacity_ = Capacity;

    if(container.memBlock_.ptr)
    {
      container.alloc_.deallocate(container.memBlock_);
    }
    container.memBlock_ = newMemBlk;

    return true;
  }
  return false;
}

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_DYN_CONTAINERS_HPP
