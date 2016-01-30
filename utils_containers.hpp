/**
  * helper functions to abstrct the backend
  *
  */

#ifndef UTILS_CONTAINERS_HPP
#define UTILS_CONTAINERS_HPP

#include <algorithm>

//#include "utils_memory.hpp"
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
template < typename ElementType, bool canOverwrite = true >
struct Array
{
  using Type = ElementType;
  const bool canOverwrite_{canOverwrite};

  cSize firstPos_;
  cSize lastPos_;
  cSize length_;
  Allocator& alloc_;

  Array() = delete;
  explicit Array(const ElementType& init, Allocator& alloc);
  explicit Array(const Array& other);
  Array& operator=(const Array& other);
  // TODO: Move constructor???
};

// implements constructors
template < typename ElementType, bool canOverwrite >
Array< ElementType, canOverwrite >::Array(const ElementType& init, Allocator& alloc)
    : alloc_{alloc}
    , firstPos_{0}
    , lastPos_{0}
    , length_{0}
{
}

template < typename ElementType, bool canOverwrite >
Array< ElementType, canOverwrite >::Array(const Array& other)
    : alloc_{other.alloc}
    , firstPos_{other.firstPos_}
    , lastPos_{other.lastPos_}
    , length_{other.length_}
{
}

template < typename ElementType, bool canOverwrite >
Array< ElementType, canOverwrite >& Array< ElementType, canOverwrite >::
operator=(const Array& other)
{
  this->alloc_ = other.alloc;
  this->firstPos_ = other.firstPos_;
  this->lastPos_ = other.lastPot_;
  this->length_ = std::min(other.length_, this->length_);
  return *this;
}

/*
at()
clear()
len()
*/

///////////////////////////////////////////////////////////////////////////////
// Stack : dynamic stack with random accessor
///////////////////////////////////////////////////////////////////////////////

// declarations
template < typename ElementType, bool canOverwrite = true >
struct Stack : public Array< ElementType, canOverwrite >
{
  Stack() = delete;
  explicit Stack(Allocator& alloc, const ElementType& init);
  explicit Stack(const Stack& other);
  Stack& operator=(const Stack& other);
  // TODO: Move constructor???
};
/*
push()
pop()
top()
bot()
*/

///////////////////////////////////////////////////////////////////////////////
// DEQ : dynamic double ended queue with random accessor
///////////////////////////////////////////////////////////////////////////////

// declarations
template < typename ElementType, bool canOverwrite = true >
struct DEQ : public Stack< ElementType, canOverwrite >
{
  DEQ() = delete;
  explicit DEQ(Allocator& alloc, const ElementType& init);
  explicit DEQ(const DEQ& other);
  DEQ& operator=(const DEQ& other);
  // TODO: Move constructor???
};
/*
push_front()
pop_front()
push_back() -> push()
pop_back() -> pop()
front() -> bot()
back() -> top()
*/

///////////////////////////////////////////////////////////////////////////////
// HashTable : dynamic hash map containter
///////////////////////////////////////////////////////////////////////////////

// declarations
template < typename ElementType, bool allowMultiple >
struct HashTable // : Array ???should this inherite from Array???
{
  const bool allowMultiple_{allowMultiple};

  HashTable() = delete;
  explicit HashTable(Allocator& alloc, const ElementType& init);
  explicit HashTable(const HashTable& other);
  HashTable& operator=(const HashTable& other);
  // TODO: Move constructor???
};
/*
??? what kind of accessor is needed ???
*/

///////////////////////////////////////////////////////////////////////////////
// Heap : dynamic heap containter
///////////////////////////////////////////////////////////////////////////////

// declarations
template < typename ElementType >
struct Heap // : Array ???should this inherite from Array???
{
  Heap() = delete;
  explicit Heap(Allocator& alloc, const ElementType& init);
  explicit Heap(const Heap& other);
  Heap& operator=(const Heap& other);
  // TODO: Move constructor???
};
/*
??? what kind of accessor is needed ???
*/

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_CONTAINERS_HPP
