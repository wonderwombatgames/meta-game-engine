/**
  * helper functions to abstrct the backend
  *
  */

#ifndef UTILS_CONTAINERS_HPP
#define UTILS_CONTAINERS_HPP

#include <algorithm>

#include "utils_memory.hpp"
#include "utils_types.hpp"

namespace W2E
{

namespace Utils
{

// forward declarations
struct Allocator;

///////////////////////////////////////////////////////////////////////////////
// Array : dynamic array with random accessor
///////////////////////////////////////////////////////////////////////////////

// declarations
template < typename ElementType, typename Allocator >
struct Array
{
  using Type = ElementType;

  // TODO: Move constructor???
  explicit Array(const ElementType& init, Allocator& alloc, const cSize capacity);
  explicit Array(const Array& other);
  ~Array();
  Array& operator=(const Array& other);
  bool resize(const cSize capacity);

  Array() = delete;
  Allocator& alloc_;
  cSize firstPos_;
  cSize lastPos_;
  cSize length_;
  cSize capacity_;
  ElementType* array_;
  ElementType init_;
  Blk memBlock_;
};

// implements constructors
template < typename ElementType, typename Allocator >
Array< ElementType, Allocator >::~Array()
{
  if(this->memBlock_.ptr)
  {
    this->alloc_.deallocate(this->memBlock_);
  }
}

template < typename ElementType, typename Allocator >
Array< ElementType, Allocator >::Array(const ElementType& init,
                                       Allocator& alloc,
                                       const cSize capacity)
    : alloc_{alloc}
    , firstPos_{0}
    , lastPos_{capacity}
    , length_{capacity}
    , capacity_{capacity}
    , array_{nullptr}
    , init_{init}
    , memBlock_{nullptr, 0}
{
  this->array_ =
      allocateType< ElementType, Allocator >(this->alloc_, this->memBlock_, this->capacity_);
  std::fill(this->array_, (this->array_ + this->capacity_), this->init_);
}

template < typename ElementType, typename Allocator >
Array< ElementType, Allocator >::Array(const Array& other)
    : alloc_{other.alloc}
    , firstPos_{other.firstPos_}
    , lastPos_{other.lastPos_}
    , length_{other.length_}
    , capacity_{other.capacity_}
    , array_{nullptr}
    , init_{other.init_}
    , memBlock_{nullptr, 0}
{
  this->array_ = allocateType(this->alloc_, this->memBlock_, this->capacity_);
  std::copy(other.array_, (other.array_ + this->capacity_), this->array_);
}

template < typename ElementType, typename Allocator >
Array< ElementType, Allocator >& Array< ElementType, Allocator >::operator=(const Array& other)
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
  this->array_ = nullptr;
  this->init_ = other.init_;
  this->memBlock_ = {nullptr, 0};

  this->array_ = allocateType(this->alloc_, this->memBlock_, this->capacity_);
  std::copy(other.array_, (other.array_ + this->capacity_), this->array_);

  return *this;
}

template < typename ElementType, typename Allocator >
bool Array< ElementType, Allocator >::resize(const cSize capacity)
{
  if(capacity > this->capacity_)
  {
    Blk newMemBlk;
    ElementType* oldArray_ = this->array_;

    this->array_ = allocateType< ElementType, Allocator >(this->alloc_, newMemBlk, capacity);
    std::fill((this->array_ + this->capacity_ - 1), (this->array_ + capacity), this->init_);
    std::copy(oldArray_, (oldArray_ + this->capacity_), this->array_);
    this->length_ = capacity;
    this->capacity_ = capacity;

    if(this->memBlock_.ptr)
    {
      this->alloc_.deallocate(this->memBlock_);
    }
    this->memBlock_ = newMemBlk;

    return true;
  }
  return false;
}

// accessor functions
template < typename ElementType, typename Allocator >
inline ElementType* at(Array< ElementType, Allocator >& container, cSize pos)
{
  m32 logicPos(container.capacity_, pos);
  if(logicPos < container.length_)
  {
    logicPos += container.firstPos_;
    return &(container.array_[logicPos.toInt()]);
  }
  return nullptr;
}

template < typename ElementType, typename Allocator >
inline cSize len(Array< ElementType, Allocator >& container)
{
  return container.length_;
}

template < typename ElementType, typename Allocator >
inline void clear(Array< ElementType, Allocator >& container, const ElementType& init)
{
  container.firstPos_ = 0;
  container.lastPos_ = container.reserve_;
  container.length_ = 0;
  std::fill(container.array_, (container.array_ + container.capacity_), init);
}

// ///////////////////////////////////////////////////////////////////////////////
// // Stack : dynamic stack with random accessor
// ///////////////////////////////////////////////////////////////////////////////
//
// // declarations
// template < typename ElementType, bool canOverwrite = true >
// struct Stack : public Array< ElementType, canOverwrite >
// {
//   Stack() = delete;
//   explicit Stack(Allocator& alloc, const ElementType& init);
//   explicit Stack(const Stack& other);
//   Stack& operator=(const Stack& other);
//   // TODO: Move constructor???
// };
// /*
// push()
// pop()
// top()
// bot()
// */
//
// ///////////////////////////////////////////////////////////////////////////////
// // DEQ : dynamic double ended queue with random accessor
// ///////////////////////////////////////////////////////////////////////////////
//
// // declarations
// template < typename ElementType, bool canOverwrite = true >
// struct DEQ : public Stack< ElementType, canOverwrite >
// {
//   DEQ() = delete;
//   explicit DEQ(Allocator& alloc, const ElementType& init);
//   explicit DEQ(const DEQ& other);
//   DEQ& operator=(const DEQ& other);
//   // TODO: Move constructor???
// };
// /*
// push_front()
// pop_front()
// push_back() -> push()
// pop_back() -> pop()
// front() -> bot()
// back() -> top()
// */
//
// ///////////////////////////////////////////////////////////////////////////////
// // HashTable : dynamic hash map containter
// ///////////////////////////////////////////////////////////////////////////////
//
// // declarations
// template < typename ElementType, bool allowMultiple >
// struct HashTable // : Array ???should this inherite from Array???
// {
//   const bool allowMultiple_{allowMultiple};
//
//   HashTable() = delete;
//   explicit HashTable(Allocator& alloc, const ElementType& init);
//   explicit HashTable(const HashTable& other);
//   HashTable& operator=(const HashTable& other);
//   // TODO: Move constructor???
// };
// /*
// ??? what kind of accessor is needed ???
// */
//
// ///////////////////////////////////////////////////////////////////////////////
// // Heap : dynamic heap containter
// ///////////////////////////////////////////////////////////////////////////////
//
// // declarations
// template < typename ElementType >
// struct Heap // : Array ???should this inherite from Array???
// {
//   Heap() = delete;
//   explicit Heap(Allocator& alloc, const ElementType& init);
//   explicit Heap(const Heap& other);
//   Heap& operator=(const Heap& other);
//   // TODO: Move constructor???
// };
// /*
// ??? what kind of accessor is needed ???
// */

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_CONTAINERS_HPP
