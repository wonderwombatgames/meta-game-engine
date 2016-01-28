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
template < typename ElementType >
struct Array
{
  using Type = ElementType;
  cSize maxLength;
  // Type array_[Capacity];

  Array() = delete;
  explicit Array(Allocator& alloc, const ElementType& init);
  explicit Array(const Array& other);
  Array& operator=(const Array& other);
  // TODO: Move constructor???
};
/*
at()
clear()
len()
*/

///////////////////////////////////////////////////////////////////////////////
// Stack : dynamic stack with random accessor
///////////////////////////////////////////////////////////////////////////////

// declarations
template < typename ElementType >
struct Stack : public Array< ElementType >
{
  cSize length_;
  cSize firstPos_;
  cSize lastPos_;

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
template < typename ElementType, bool canOverwrite >
struct DEQ : public Stack< ElementType >
{
  bool canOverwrite_{canOverwrite};

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
  bool allowMultiple_ = allowMultiple_;

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
