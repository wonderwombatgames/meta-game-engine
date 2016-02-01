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
// FixedContainer : parametric base for array, stack, and deq
///////////////////////////////////////////////////////////////////////////////

// declarations
template < typename ElementType,
           cSize Capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
struct FixedContainer
{
  using Type = ElementType;
  GLOBAL const cSize maxLength_;
  GLOBAL const bool canOverwrite_;

  cSize firstPos_;
  cSize lastPos_;
  cSize length_;
  Type array_[Capacity];

  FixedContainer() = delete;
  explicit FixedContainer(const ElementType& init);
  FixedContainer(const FixedContainer& other);
  FixedContainer& operator=(const FixedContainer& other);
  // TODO: Move constructor???
};

// GLOBAL
template < typename ElementType,
           cSize Capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
const cSize
    FixedContainer< ElementType, Capacity, initialLen, initialPos, finalPos, canOverwrite >::
        maxLength_{Capacity};

template < typename ElementType,
           cSize Capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
const bool FixedContainer< ElementType, Capacity, initialLen, initialPos, finalPos, canOverwrite >::
    canOverwrite_{canOverwrite};

// implements constructors
template < typename ElementType,
           cSize Capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
FixedContainer< ElementType, Capacity, initialLen, initialPos, finalPos, canOverwrite >::
    FixedContainer(const ElementType& init)
    : firstPos_{initialPos}
    , lastPos_{finalPos}
    , length_{initialLen}
    , array_{}
{
  std::fill(array_, (array_ + maxLength_), init);
}

// constructors
template < typename ElementType,
           cSize Capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
FixedContainer< ElementType, Capacity, initialLen, initialPos, finalPos, canOverwrite >::
    FixedContainer(const FixedContainer& other)
    : firstPos_{other.firstPos_}
    , lastPos_{other.lastPos_}
    , length_{other.length_}
    , array_{}
{
  std::copy(other.array_, (other.array_ + std::min(other.maxLength_, maxLength_)), array_);
}

template < typename ElementType,
           cSize Capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
FixedContainer< ElementType, Capacity, initialLen, initialPos, finalPos, canOverwrite >&
FixedContainer< ElementType, Capacity, initialLen, initialPos, finalPos, canOverwrite >::
operator=(const FixedContainer& other)
{
  std::copy(
      other.array_, (other.array_ + std::min(other.maxLength_, this->maxLength_)), this->array_);
  this->firstPos_ = other.firstPos_;
  this->lastPos_ = other.lastPot_;
  this->length_ = std::min(other.length_, this->length_);
  return *this;
}

// accessor fucntions
template < typename ElementType,
           cSize Capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
inline ElementType*
at(FixedContainer< ElementType, Capacity, initialLen, initialPos, finalPos, canOverwrite >&
       container,
   cSize pos)
{
  if(pos < container.length_)
  {
    return &(container.array_[(pos + container.firstPos_) % container.maxLength_]);
  }
  return nullptr;
}

template < typename ElementType,
           cSize Capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
inline cSize
len(FixedContainer< ElementType, Capacity, initialLen, initialPos, finalPos, canOverwrite >&
        container)
{
  return container.length_;
}

template < typename ElementType,
           cSize Capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
inline void
clear(FixedContainer< ElementType, Capacity, initialLen, initialPos, finalPos, canOverwrite >&
          container,
      const ElementType& init)
{
  std::fill(container.array_, (container.array_ + container.maxLength_), init);
  container.firstPos_ = initialPos;
  container.lastPos_ = finalPos;
  container.length_ = initialLen;
}

///////////////////////////////////////////////////////////////////////////////
// Array : Fixed size Array with random accessor
///////////////////////////////////////////////////////////////////////////////

template < typename ElementType, cSize Capacity >
using FixedArray = FixedContainer< ElementType, Capacity, Capacity, 0, 0, false >;

///////////////////////////////////////////////////////////////////////////////
// Stack : Fixed size Stack with random accessor
///////////////////////////////////////////////////////////////////////////////

// template < typename ElementType, cSize Capacity >
// using FixedStack = FixedDEQ< ElementType, Capacity, false >;
template < typename ElementType, cSize Capacity, bool canOverwrite = false >
using FixedStack =
    FixedContainer< ElementType, Capacity, 0, (Capacity >> 1), (Capacity >> 1), canOverwrite >;

///////////////////////////////////////////////////////////////////////////////
// DEQueue : Fixed size double ended queue with random accessor
///////////////////////////////////////////////////////////////////////////////

template < typename ElementType, cSize Capacity, bool canOverwrite = false >
using FixedDEQ = FixedStack< ElementType, Capacity, canOverwrite >;

///////////////////////////////////////////////////////////////////////////////
// RingQ : fixed size, acessible back and front with overwriting
///////////////////////////////////////////////////////////////////////////////

template < typename ElementType, cSize Capacity >
using FixedRingQ = FixedDEQ< ElementType, Capacity, true >;

///////////////////////////////////////////////////////////////////////////////

// Stack :
// accessor methods
template < typename ElementType, cSize Capacity, bool canOverwrite >
inline ErrorCode push(FixedStack< ElementType, Capacity, canOverwrite >& container,
                      const ElementType& el)
{
  if((container.length_ < container.maxLength_) || container.canOverwrite_)
  {
    if(container.length_ > 0)
    {
      container.lastPos_ = (container.lastPos_ + 1) % container.maxLength_;
      if(container.lastPos_ == container.firstPos_)
      {
        container.firstPos_ = (container.firstPos_ + 1) % container.maxLength_;
      }
    }
    container.array_[container.lastPos_] = el;
    container.length_ = std::min(container.length_ + 1, container.maxLength_);
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

template < typename ElementType, cSize Capacity, bool canOverwrite >
inline ElementType pop(FixedStack< ElementType, Capacity, canOverwrite >& container,
                       ElementType fallback)
{
  if(container.length_ > 0)
  {
    cSize previousPos = container.lastPos_;
    --container.length_;
    container.lastPos_ = (container.lastPos_ + container.maxLength_ - 1) % container.maxLength_;
    return container.array_[previousPos];
  }
  return fallback;
}

template < typename ElementType, cSize Capacity, bool canOverwrite >
inline ElementType* bot(FixedStack< ElementType, Capacity, canOverwrite >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[container.firstPos_]);
  }
  return nullptr;
}

template < typename ElementType, cSize Capacity, bool canOverwrite >
inline ElementType* top(FixedStack< ElementType, Capacity, canOverwrite >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[container.lastPos_]);
  }
  return nullptr;
}

// DEQueue :
// accessor methods
template < typename ElementType, cSize Capacity, bool canOverwrite >
inline ErrorCode push_back(FixedDEQ< ElementType, Capacity, canOverwrite >& container,
                           const ElementType& el)
{
  return push(container, el);
}

template < typename ElementType, cSize Capacity, bool canOverwrite >
inline ErrorCode push_front(FixedDEQ< ElementType, Capacity, canOverwrite >& container,
                            const ElementType& el)
{
  if((container.length_ < container.maxLength_) || container.canOverwrite_)
  {
    if(container.length_ > 0)
    {
      container.firstPos_ = (container.firstPos_ + container.maxLength_ - 1) % container.maxLength_;
      if(container.lastPos_ == container.firstPos_)
      {
        container.lastPos_ = (container.lastPos_ + container.maxLength_ - 1) % container.maxLength_;
      }
    }
    container.array_[container.firstPos_] = el;
    container.length_ = std::min(container.length_ + 1, container.maxLength_);
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

template < typename ElementType, cSize Capacity, bool canOverwrite >
inline ElementType pop_back(FixedDEQ< ElementType, Capacity, canOverwrite >& container,
                            ElementType fallback)
{
  return pop(container, fallback);
}

template < typename ElementType, cSize Capacity, bool canOverwrite >
inline ElementType pop_front(FixedDEQ< ElementType, Capacity, canOverwrite >& container,
                             ElementType fallback)
{
  if(container.length_ > 0)
  {
    cSize previousPos = container.firstPos_;
    --container.length_;
    container.firstPos_ = (container.firstPos_ + 1) % container.maxLength_;
    return container.array_[previousPos];
  }
  return fallback;
}

template < typename ElementType, cSize Capacity, bool canOverwrite >
inline ElementType* front(FixedDEQ< ElementType, Capacity, canOverwrite >& container)
{
  return bot(container);
}

template < typename ElementType, cSize Capacity, bool canOverwrite >
inline ElementType* back(FixedDEQ< ElementType, Capacity, canOverwrite >& container)
{
  return top(container);
}

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_FIXCONTAINERS_HPP
