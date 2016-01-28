/**
  * helper functions to abstrct the backend
  *
  */

#ifndef UTILS_FIXCONTAINERS_HPP
#define UTILS_FIXCONTAINERS_HPP

#include <algorithm>

#include "utils_types.hpp"

namespace W2E
{

namespace Utils
{

///////////////////////////////////////////////////////////////////////////////
// Array : Fixed size Array with random accessor
///////////////////////////////////////////////////////////////////////////////

// declarations
template < typename ElementType, cSize Capacity >
struct FixedArray
{
  using Type = ElementType;
  const cSize maxLength_{Capacity};
  Type array_[Capacity];

  FixedArray() = delete;
  explicit FixedArray(const ElementType& init);
  FixedArray(const FixedArray& other);
  // TODO: Move constructor???
  FixedArray& operator=(const FixedArray& other);
};

// implements the constructors
template < typename ElementType, cSize Capacity >
FixedArray< ElementType, Capacity >::FixedArray(const ElementType& init)
    : array_{}
{
  std::fill(array_, (array_ + maxLength_), init);
}

template < typename ElementType, cSize Capacity >
FixedArray< ElementType, Capacity >::FixedArray(const FixedArray& other)
    : array_{}
{
  std::copy(other.array_, (other.array_ + std::min(other.maxLength_, maxLength_)), array_);
}

template < typename ElementType, cSize Capacity >
FixedArray< ElementType, Capacity >& FixedArray< ElementType, Capacity >::
operator=(const FixedArray& other)
{
  std::copy(other.array_, (other.array_ + std::min(other.maxLength_, maxLength_)), array_);
  return *this;
}

// accessor methods

template < typename ElementType, cSize Capacity >
bool equalUpTo(FixedArray< ElementType, Capacity >& container,
               const FixedArray< ElementType, Capacity >& other)
{
  return std::equal(other.array_,
                    (other.array_ + std::min(other.maxLength_, container.maxLength_)),
                    container.array_);
}

template < typename ElementType, cSize Capacity >
ElementType* at(FixedArray< ElementType, Capacity >& container, cSize pos)
{
  if(pos < container.maxLength_)
  {
    return &(container.array_[pos]);
  }
  return nullptr;
}

template < typename ElementType, cSize Capacity >
void clear(FixedArray< ElementType, Capacity >& container, const ElementType& init)
{
  std::fill(container.array_, (container.array_ + container.maxLength_), init);
}

template < typename ElementType, cSize Capacity >
cSize length(FixedArray< ElementType, Capacity >& container)
{
  return container.maxLength_;
}

///////////////////////////////////////////////////////////////////////////////
// Stack : Fixed size Stack with random accessor
///////////////////////////////////////////////////////////////////////////////

// declarations
template < typename ElementType, cSize Capacity >
struct FixedStack
{
  using Type = ElementType;
  const cSize maxLength_{Capacity};
  Type array_[Capacity];
  cSize length_;
  cSize lastPos_;

  FixedStack() = delete;
  explicit FixedStack(const ElementType& init);
  FixedStack(const FixedStack& other);
  // TODO: Move constructor???
  FixedStack& operator=(const FixedStack& other);
};

// implements the constructors
template < typename ElementType, cSize Capacity >
FixedStack< ElementType, Capacity >::FixedStack(const ElementType& init)
    : array_{}
    , length_{0}
    , lastPos_{0}
{
  std::fill(array_, (array_ + maxLength_), init);
}

template < typename ElementType, cSize Capacity >
FixedStack< ElementType, Capacity >::FixedStack(const FixedStack& other)
    : array_{}
    , length_{other.length_}
    , lastPos_{other.lastPos}
{
  std::copy(other.array_, (other.array_ + std::min(other.maxLength_, maxLength_)), array_);
}

template < typename ElementType, cSize Capacity >
FixedStack< ElementType, Capacity >& FixedStack< ElementType, Capacity >::
operator=(const FixedStack& other)
{
  length_ = other.length_;
  lastPos_ = other.lastPos_;
  std::copy(other.array_, (other.array_ + std::min(other.maxLength_, maxLength_)), array_);
  return *this;
}

// accessor methods
template < typename ElementType, cSize Capacity >
ErrorCode push(FixedStack< ElementType, Capacity >& container, const ElementType& el)
{
  if(container.length_ < container.maxLength_)
  {
    if(container.length_ > 0)
    {
      ++container.lastPos_;
    }
    container.array_[container.lastPos_] = el;
    ++container.length_;
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

template < typename ElementType, cSize Capacity >
ElementType pop(FixedStack< ElementType, Capacity >& container, ElementType fallback)
{
  if(container.length_ > 0)
  {
    --container.length_;
    --container.lastPos_;
    return container.array_[container.lastPos_ + 1];
  }
  return fallback;
}

template < typename ElementType, cSize Capacity >
ElementType* first(FixedStack< ElementType, Capacity >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[0]);
  }
  return nullptr;
}

template < typename ElementType, cSize Capacity >
ElementType* last(FixedStack< ElementType, Capacity >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[container.lastPos_]);
  }
  return nullptr;
}

template < typename ElementType, cSize Capacity >
ElementType* at(FixedStack< ElementType, Capacity >& container, cSize pos)
{
  if(pos < container.length_)
  {
    return &(container.array_[pos]);
  }
  return nullptr;
}

template < typename ElementType, cSize Capacity >
void clear(FixedStack< ElementType, Capacity >& container, const ElementType& init)
{
  std::fill(container.array_, (container.array_ + container.maxLength_), init);
  container.length_ = 0;
  container.lastPos_ = 0;
}

template < typename ElementType, cSize Capacity >
cSize length(FixedStack< ElementType, Capacity >& container)
{
  return container.length_;
}

///////////////////////////////////////////////////////////////////////////////
// RingQ : Fixed size circular queue with random accessor
///////////////////////////////////////////////////////////////////////////////

// declarations
// FIXME: can be implemented with the DEQ supporting overwrite!!!
// template < typename ElementType, cSize Capacity >
// struct FixedRingQ
// {
//   using Type = ElementType;
//   const cSize maxLength_{Capacity};
//   Type array_[Capacity];
//   cSize length_;
//   cSize firstPos_;
//   cSize lastPos_;
//
//   FixedRingQ() = delete;
//   explicit FixedRingQ(const ElementType& init);
//   FixedRingQ(const FixedRingQ& other);
//   // TODO: Move constructor???
//   FixedRingQ& operator=(const FixedRingQ& other);
// };
//
// // implements the constructors
// template < typename ElementType, cSize Capacity >
// FixedRingQ< ElementType, Capacity >::FixedRingQ(const ElementType& init)
//     : array_{}
//     , length_{0}
//     , firstPos_{0}
//     , lastPos_{0}
// {
//   std::fill(array_, (array_ + maxLength_), init);
// }
//
// template < typename ElementType, cSize Capacity >
// FixedRingQ< ElementType, Capacity >::FixedRingQ(const FixedRingQ& other)
//     : array_{}
//     , length_{other.length_}
//     , firstPos_{other.firstPos_}
//     , lastPos_{other.lastPos}
// {
//   std::copy(other.array_, (other.array_ + std::min(other.maxLength_, maxLength_)), array_);
// }
//
// template < typename ElementType, cSize Capacity >
// FixedRingQ< ElementType, Capacity >& FixedRingQ< ElementType, Capacity >::
// operator=(const FixedRingQ& other)
// {
//   length_ = other.length_;
//   lastPos_ = other.lastPos_;
//   firstPos_ = other.firstPos_;
//
//   std::copy(other.array_, (other.array_ + std::min(other.maxLength_, maxLength_)), array_);
//   return *this;
// }

// accessor functions
// TODO

///

///////////////////////////////////////////////////////////////////////////////
// DEQueue : fixex size double ended queue with random accessor
///////////////////////////////////////////////////////////////////////////////

// declarations
template < typename ElementType, cSize Capacity >
struct FixedDEQ
{
  using Type = ElementType;
  const cSize maxLength_{Capacity};
  Type array_[Capacity];
  cSize length_;
  cSize firstPos_;
  cSize lastPos_;

  FixedDEQ() = delete;
  explicit FixedDEQ(const ElementType& init);
  FixedDEQ(const FixedDEQ& other);
  // TODO: Move constructor???
  FixedDEQ& operator=(const FixedDEQ& other);
};

// implements the constructors
template < typename ElementType, cSize Capacity >
FixedDEQ< ElementType, Capacity >::FixedDEQ(const ElementType& init)
    : array_{}
    , length_{0}
    , firstPos_{Capacity >> 1}
    , lastPos_{Capacity >> 1}
{
  std::fill(array_, (array_ + maxLength_), init);
}

template < typename ElementType, cSize Capacity >
FixedDEQ< ElementType, Capacity >::FixedDEQ(const FixedDEQ& other)
    : array_{}
    , length_{other.length}
    , firstPos_{other.firstPos}
    , lastPos_{other.lastPos}
{
  std::copy(other.array_, (other.array_ + std::min(other.maxLength_, maxLength_)), array_);
}

template < typename ElementType, cSize Capacity >
FixedDEQ< ElementType, Capacity >& FixedDEQ< ElementType, Capacity >::
operator=(const FixedDEQ& other)
{
  length_ = other.length;
  firstPos_ = other.firstPos;
  lastPos_ = other.lastPos;
  std::copy(other.array_, (other.array_ + std::min(other.maxLength_, maxLength_)), array_);
  return *this;
}

// accessor methods
template < typename ElementType, cSize Capacity >
ErrorCode push_back(FixedDEQ< ElementType, Capacity >& container, const ElementType& el)
{
  if(container.length_ < container.maxLength_)
  {
    container.lastPos_ = (container.lastPos_ + 1) % container.maxLength_;
    container.array_[container.lastPos_] = el;
    if(container.length_ == 0)
    {
      container.firstPos_ = container.lastPos_;
    }
    ++container.length_;
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

template < typename ElementType, cSize Capacity >
ErrorCode push_front(FixedDEQ< ElementType, Capacity >& container, const ElementType& el)
{
  if(container.length_ < container.maxLength_)
  {
    container.firstPos_ = (container.firstPos_ - 1 + container.maxLength_) % container.maxLength_;
    container.array_[container.firstPos_] = el;
    if(container.length_ == 0)
    {
      container.lastPos_ = container.firstPos_;
    }
    ++container.length_;
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

template < typename ElementType, cSize Capacity >
ElementType pop_back(FixedDEQ< ElementType, Capacity >& container, ElementType fallback)
{
  if(container.length_ > 0)
  {
    --container.length_;
    --container.lastPos_;
    return container.array_[container.lastPos_ + 1];
  }
  return fallback;
}

template < typename ElementType, cSize Capacity >
ElementType pop_front(FixedDEQ< ElementType, Capacity >& container, ElementType fallback)
{
  if(container.length_ > 0)
  {
    --container.length_;
    ++container.firstPos_;
    return container.array_[container.firstPos_ - 1];
  }
  return fallback;
}

template < typename ElementType, cSize Capacity >
ElementType* front(FixedDEQ< ElementType, Capacity >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[container.firstPos_]);
  }
  return nullptr;
}

template < typename ElementType, cSize Capacity >
ElementType* back(FixedDEQ< ElementType, Capacity >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[container.lastPos_]);
  }
  return nullptr;
}

template < typename ElementType, cSize Capacity >
ElementType* at(FixedDEQ< ElementType, Capacity >& container, cSize pos)
{
  if(pos < container.length_)
  {
    cSize index = (pos + container.firstPos_) % container.maxLength_;
    return &(container.array_[index]);
  }
  return nullptr;
}

template < typename ElementType, cSize Capacity >
void clear(FixedDEQ< ElementType, Capacity >& container, const ElementType& init)
{
  std::fill(container.array_, (container.array_ + container.maxLength_), init);
  container.length_ = 0;
  container.firstPos_ = Capacity >> 1;
  container.lastPos_ = container.firstPos_;
}

template < typename ElementType, cSize Capacity >
cSize length(FixedDEQ< ElementType, Capacity >& container)
{
  return container.length_;
}

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_FIXCONTAINERS_HPP
