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

// TODO: Do we need a BitMap container??? Probably we do...

///////////////////////////////////////////////////////////////////////////////
// FixedContainer : parametric base for array, stack, and deq
///////////////////////////////////////////////////////////////////////////////

// declarations
template < typename ElementType, cSize capacity, cSize initialLen, bool canOverwrite >
struct FixedContainer
{
  using Type = ElementType;
  GLOBAL const cSize capacity_;
  GLOBAL const bool canOverwrite_;

  cSize firstPos_;
  cSize lastPos_;
  cSize length_;
  Type array_[capacity];

  FixedContainer() = delete;
  explicit FixedContainer(const ElementType& init);
  FixedContainer(const FixedContainer& other);
  FixedContainer& operator=(const FixedContainer& other);
  // TODO: Move constructor???
};

// GLOBAL
template < typename ElementType, cSize capacity, cSize initialLen, bool canOverwrite >
const cSize FixedContainer< ElementType, capacity, initialLen, canOverwrite >::capacity_{capacity};

template < typename ElementType, cSize capacity, cSize initialLen, bool canOverwrite >
const bool FixedContainer< ElementType, capacity, initialLen, canOverwrite >::canOverwrite_{
    canOverwrite};

// implements constructors
template < typename ElementType, cSize capacity, cSize initialLen, bool canOverwrite >
FixedContainer< ElementType, capacity, initialLen, canOverwrite >::FixedContainer(
    const ElementType& init)
    : firstPos_{0}
    , lastPos_{initialLen}
    , length_{initialLen}
    , array_{}
{
  std::fill(this->array_, (this->array_ + this->capacity_), init);
}

// constructors
template < typename ElementType, cSize capacity, cSize initialLen, bool canOverwrite >
FixedContainer< ElementType, capacity, initialLen, canOverwrite >::FixedContainer(
    const FixedContainer& other)
    : firstPos_{other.firstPos_}
    , lastPos_{other.lastPos_}
    , length_{other.length_}
    , array_{}
{
  std::copy(
      other.array_, (other.array_ + std::min(other.capacity_, this->capacity_)), this->array_);
}

template < typename ElementType, cSize capacity, cSize initialLen, bool canOverwrite >
FixedContainer< ElementType, capacity, initialLen, canOverwrite >&
FixedContainer< ElementType, capacity, initialLen, canOverwrite >::
operator=(const FixedContainer& other)
{
  std::copy(
      other.array_, (other.array_ + std::min(other.capacity_, this->capacity_)), this->array_);
  this->firstPos_ = other.firstPos_;
  this->lastPos_ = other.lastPot_;
  this->length_ = std::min(other.length_, this->length_.toInt());
  return *this;
}

// accessor fucntions
template < typename ElementType, cSize capacity, cSize initialLen, bool canOverwrite >
inline ElementType* at(FixedContainer< ElementType, capacity, initialLen, canOverwrite >& container,
                       cSize pos)
{
  m64 logicPos(capacity, pos);
  if(logicPos < container.length_)
  {
    logicPos += container.firstPos_;
    return &(container.array_[logicPos.toInt()]);
  }
  return nullptr;
}

template < typename ElementType, cSize capacity, cSize initialLen, bool canOverwrite >
inline cSize len(FixedContainer< ElementType, capacity, initialLen, canOverwrite >& container)
{
  return container.length_;
}

template < typename ElementType, cSize capacity, cSize initialLen, bool canOverwrite >
inline void clear(FixedContainer< ElementType, capacity, initialLen, canOverwrite >& container,
                  const ElementType& init)
{
  std::fill(container.array_, (container.array_ + container.capacity_), init);
  container.firstPos_ = 0;
  container.lastPos_ = initialLen;
  container.length_ = initialLen;
}

///////////////////////////////////////////////////////////////////////////////
// Array : Fixed size Array with random accessor
///////////////////////////////////////////////////////////////////////////////

template < typename ElementType, cSize capacity >
using FixedArray = FixedContainer< ElementType, capacity, capacity, false >;

///////////////////////////////////////////////////////////////////////////////
// DEQueue : Fixed size double ended queue with random accessor
///////////////////////////////////////////////////////////////////////////////

template < typename ElementType, cSize capacity, bool canOverwrite = false >
using FixedDEQ = FixedContainer< ElementType, capacity, 0, canOverwrite >;

///////////////////////////////////////////////////////////////////////////////
// RingQ : fixed size, acessible back and front with overwriting
///////////////////////////////////////////////////////////////////////////////

template < typename ElementType, cSize capacity >
using FixedRingQ = FixedDEQ< ElementType, capacity, true >;

///////////////////////////////////////////////////////////////////////////////

// accessor methods
// DEQ :
template < typename ElementType, cSize capacity, bool canOverwrite >
inline ErrorCode push(FixedDEQ< ElementType, capacity, canOverwrite >& container,
                      const ElementType& el)
{
  if((container.length_ < container.capacity_) || container.canOverwrite_)
  {
    m64 mm(container.capacity_, 0);
    if(container.length_ > 0)
    {
      mm = container.lastPos_;
      container.lastPos_ = (++mm).toInt();
      if(container.lastPos_ == container.firstPos_)
      {
        mm = container.firstPos_;
        container.firstPos_ = (++mm).toInt();
      }
    }
    container.array_[container.lastPos_] = el;
    const cSize currLen = container.length_ + 1;
    container.length_ = std::min(currLen, container.capacity_);
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

template < typename ElementType, cSize capacity, bool canOverwrite >
inline ElementType pop(FixedDEQ< ElementType, capacity, canOverwrite >& container,
                       ElementType fallback)
{
  m64 mm(container.capacity_, 0);
  if(container.length_ > 0)
  {
    cSize previousPos = container.lastPos_;
    mm = container.length_;
    container.length_ = (--mm).toInt();
    mm = container.lastPos_;
    container.lastPos_ = (--mm).toInt();
    return container.array_[previousPos];
  }
  return fallback;
}

template < typename ElementType, cSize capacity, bool canOverwrite >
inline ElementType* bot(FixedDEQ< ElementType, capacity, canOverwrite >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[container.firstPos_]);
  }
  return nullptr;
}

template < typename ElementType, cSize capacity, bool canOverwrite >
inline ElementType* top(FixedDEQ< ElementType, capacity, canOverwrite >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[container.lastPos_]);
  }
  return nullptr;
}

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
  m64 mm(container.capacity_, 0);
  if((container.length_ < container.capacity_) || container.canOverwrite_)
  {
    if(container.length_ > 0)
    {
      mm = container.firstPos_;
      container.firstPos_ = (--mm).toInt();
      if(container.lastPos_ == container.firstPos_)
      {
        mm = container.lastPos_;
        container.lastPos_ = (--mm).toInt();
      }
    }
    container.array_[container.firstPos_] = el;
    const cSize currLen = container.length_ + 1;
    container.length_ = std::min(currLen, container.capacity_);
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
  m64 mm(container.capacity_, 0);
  if(container.length_ > 0)
  {
    cSize previousPos = container.firstPos_;

    mm = container.length_;
    container.length_ = (--mm).toInt();

    mm = container.firstPos_;
    container.firstPos_ = (++mm).toInt();

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
