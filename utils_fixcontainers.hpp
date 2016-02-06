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
           cSize capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
struct FixedContainer
{
  using Type = ElementType;
  using IndexType = Modulus64< capacity >;
  GLOBAL const cSize maxLength_;
  GLOBAL const bool canOverwrite_;

  IndexType firstPos_;
  IndexType lastPos_;
  Modulus32< capacity + 1 > length_;
  Type array_[capacity];

  FixedContainer() = delete;
  explicit FixedContainer(const ElementType& init);
  FixedContainer(const FixedContainer& other);
  FixedContainer& operator=(const FixedContainer& other);
  // TODO: Move constructor???
};

// GLOBAL
template < typename ElementType,
           cSize capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
const cSize
    FixedContainer< ElementType, capacity, initialLen, initialPos, finalPos, canOverwrite >::
        maxLength_{capacity};

template < typename ElementType,
           cSize capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
const bool FixedContainer< ElementType, capacity, initialLen, initialPos, finalPos, canOverwrite >::
    canOverwrite_{canOverwrite};

// implements constructors
template < typename ElementType,
           cSize capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
FixedContainer< ElementType, capacity, initialLen, initialPos, finalPos, canOverwrite >::
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
           cSize capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
FixedContainer< ElementType, capacity, initialLen, initialPos, finalPos, canOverwrite >::
    FixedContainer(const FixedContainer& other)
    : firstPos_{other.firstPos_}
    , lastPos_{other.lastPos_}
    , length_{other.length_}
    , array_{}
{
  std::copy(other.array_, (other.array_ + std::min(other.maxLength_, maxLength_)), array_);
}

template < typename ElementType,
           cSize capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
FixedContainer< ElementType, capacity, initialLen, initialPos, finalPos, canOverwrite >&
FixedContainer< ElementType, capacity, initialLen, initialPos, finalPos, canOverwrite >::
operator=(const FixedContainer& other)
{
  std::copy(
      other.array_, (other.array_ + std::min(other.maxLength_, this->maxLength_)), this->array_);
  this->firstPos_ = other.firstPos_;
  this->lastPos_ = other.lastPot_;
  this->length_ = std::min(other.length_, this->length_.toInt());
  return *this;
}

// accessor fucntions
template < typename ElementType,
           cSize capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
inline ElementType*
at(FixedContainer< ElementType, capacity, initialLen, initialPos, finalPos, canOverwrite >&
       container,
   cSize pos)
{
  typename FixedContainer< ElementType, capacity, initialLen, initialPos, finalPos, canOverwrite >::
      IndexType logicPos(pos);
  if(logicPos.toInt() < container.length_.toInt())
  {
    logicPos += container.firstPos_;
    return &(container.array_[logicPos.toInt()]);
  }
  return nullptr;
}

template < typename ElementType,
           cSize capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
inline cSize
len(FixedContainer< ElementType, capacity, initialLen, initialPos, finalPos, canOverwrite >&
        container)
{
  return container.length_.toInt();
}

template < typename ElementType,
           cSize capacity,
           cSize initialLen,
           cSize initialPos,
           cSize finalPos,
           bool canOverwrite >
inline void
clear(FixedContainer< ElementType, capacity, initialLen, initialPos, finalPos, canOverwrite >&
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

template < typename ElementType, cSize capacity >
using FixedArray = FixedContainer< ElementType, capacity, capacity, 0, 0, false >;

///////////////////////////////////////////////////////////////////////////////
// Stack : Fixed size Stack with random accessor
///////////////////////////////////////////////////////////////////////////////

template < typename ElementType, cSize capacity, bool canOverwrite = false >
using FixedStack =
    FixedContainer< ElementType, capacity, 0, (capacity >> 1), (capacity >> 1), canOverwrite >;

///////////////////////////////////////////////////////////////////////////////
// DEQueue : Fixed size double ended queue with random accessor
///////////////////////////////////////////////////////////////////////////////

template < typename ElementType, cSize capacity, bool canOverwrite = false >
using FixedDEQ = FixedStack< ElementType, capacity, canOverwrite >;

///////////////////////////////////////////////////////////////////////////////
// RingQ : fixed size, acessible back and front with overwriting
///////////////////////////////////////////////////////////////////////////////

template < typename ElementType, cSize capacity >
using FixedRingQ = FixedDEQ< ElementType, capacity, true >;

///////////////////////////////////////////////////////////////////////////////

// Stack :
// accessor methods
template < typename ElementType, cSize capacity, bool canOverwrite >
inline ErrorCode push(FixedStack< ElementType, capacity, canOverwrite >& container,
                      const ElementType& el)
{
  if((container.length_.toInt() < container.maxLength_) || container.canOverwrite_)
  {
    if(container.length_ > 0)
    {
      container.lastPos_ += 1;
      if(container.lastPos_ == container.firstPos_)
      {
        container.firstPos_ += 1;
      }
    }
    container.array_[container.lastPos_.toInt()] = el;
    const cSize currLen = container.length_.toInt() + 1;
    container.length_ = std::min(currLen, container.maxLength_);
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

template < typename ElementType, cSize capacity, bool canOverwrite >
inline ElementType pop(FixedStack< ElementType, capacity, canOverwrite >& container,
                       ElementType fallback)
{
  if(container.length_ > 0)
  {
    cSize previousPos = container.lastPos_.toInt();
    container.length_ -= 1;
    container.lastPos_ -= 1;
    return container.array_[previousPos];
  }
  return fallback;
}

template < typename ElementType, cSize capacity, bool canOverwrite >
inline ElementType* bot(FixedStack< ElementType, capacity, canOverwrite >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[container.firstPos_.toInt()]);
  }
  return nullptr;
}

template < typename ElementType, cSize capacity, bool canOverwrite >
inline ElementType* top(FixedStack< ElementType, capacity, canOverwrite >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[container.lastPos_.toInt()]);
  }
  return nullptr;
}

// DEQ :
// accessor methods
template < typename ElementType, cSize capacity, bool canOverwrite >
inline ErrorCode push_back(FixedDEQ< ElementType, capacity, canOverwrite >& container,
                           const ElementType& el)
{
  return push(container, el);
}

template < typename ElementType, cSize capacity, bool canOverwrite >
inline ErrorCode push_front(FixedDEQ< ElementType, capacity, canOverwrite >& container,
                            const ElementType& el)
{
  if((container.length_ < container.maxLength_) || container.canOverwrite_)
  {
    if(container.length_ > 0)
    {
      container.firstPos_ -= 1;
      if(container.lastPos_ == container.firstPos_)
      {
        container.lastPos_ -= 1;
      }
    }
    container.array_[container.firstPos_.toInt()] = el;
    const cSize currLen = container.length_.toInt() + 1;
    container.length_ = std::min(currLen, container.maxLength_);
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

template < typename ElementType, cSize capacity, bool canOverwrite >
inline ElementType pop_back(FixedDEQ< ElementType, capacity, canOverwrite >& container,
                            ElementType fallback)
{
  return pop(container, fallback);
}

template < typename ElementType, cSize capacity, bool canOverwrite >
inline ElementType pop_front(FixedDEQ< ElementType, capacity, canOverwrite >& container,
                             ElementType fallback)
{
  if(container.length_ > 0)
  {
    cSize previousPos = container.firstPos_.toInt();
    container.length_ -= 1;
    container.firstPos_ += 1;
    return container.array_[previousPos];
  }
  return fallback;
}

template < typename ElementType, cSize capacity, bool canOverwrite >
inline ElementType* front(FixedDEQ< ElementType, capacity, canOverwrite >& container)
{
  return bot(container);
}

template < typename ElementType, cSize capacity, bool canOverwrite >
inline ElementType* back(FixedDEQ< ElementType, capacity, canOverwrite >& container)
{
  return top(container);
}

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_FIXCONTAINERS_HPP
