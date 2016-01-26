/**
  * helper functions to abstrct the backend
  *
  */

/// FIXME: simplify!!! make it work just as a ring buffer!
/// TODO: break into 2 containers: stack and de-queue

#ifndef UTILS_CONTAINERS_HPP
#define UTILS_CONTAINERS_HPP

#include "utils_types.hpp"

namespace W2E
{

namespace Utils
{

using cSize = u32;

// declarations
template < typename ElementType, cSize Capacity >
struct FixedDEQueue
{
  using Type = ElementType;
  const cSize maxLength = Capacity;

  FixedDEQueue() = delete;
  explicit FixedDEQueue(const ElementType& init);
  FixedDEQueue(const FixedDEQueue& other);
  FixedDEQueue& operator=(const FixedDEQueue& other);

  Type array_[Capacity];
  cSize length_;
  cSize internalLength_;
  cSize firstPos_;
  cSize lastPos_;
};

// implents the constructors
template < typename ElementType, cSize Capacity >
FixedDEQueue< ElementType, Capacity >::FixedDEQueue(const ElementType& init)
    : array_{}
    , length_{0}
    , internalLength_{0}
    , firstPos_{Capacity >> 1}
    , lastPos_{Capacity >> 1}
{
  std::fill(array_, array_ + Capacity, init);
}

template < typename ElementType, cSize Capacity >
FixedDEQueue< ElementType, Capacity >::FixedDEQueue(const FixedDEQueue& other)
    : array_{}
    , length_{other.length}
    , internalLength_{other.nternalLentgth}
    , firstPos_{other.firstPos}
    , lastPos_{other.lastPos}
{
  std::copy(other.array_, other.array_ + other.internalLength, array_);
}

template < typename ElementType, cSize Capacity >
FixedDEQueue< ElementType, Capacity >& FixedDEQueue< ElementType, Capacity >::
operator=(const FixedDEQueue& other)
{
  length_ = other.length;
  internalLength_ = other.internalLentgth;
  firstPos_ = other.firstPos;
  lastPos_ = other.lastPos;
  std::copy(other.array_, other.array_ + other.internalLength, array_);
  return *this;
}

// accessor methods

template < typename ElementType, cSize Capacity >
ErrorCode push_back(FixedDEQueue< ElementType, Capacity >& container, const ElementType& el)
{
  if(container.length_ < container.maxLength)
  {
    container.lastPos_ = (container.lastPos_ + 1) % container.maxLength;
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
ErrorCode push_front(FixedDEQueue< ElementType, Capacity >& container, const ElementType& el)
{
  if(container.length_ < container.maxLength)
  {
    container.firstPos_ = (container.firstPos_ - 1 + container.maxLength) % container.maxLength;
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
ElementType pop_back(FixedDEQueue< ElementType, Capacity >& container, ElementType fallback)
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
ElementType pop_front(FixedDEQueue< ElementType, Capacity >& container, ElementType fallback)
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
ElementType* front(FixedDEQueue< ElementType, Capacity >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[container.firstPos_]);
  }
  return nullptr;
}

template < typename ElementType, cSize Capacity >
ElementType* back(FixedDEQueue< ElementType, Capacity >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[container.lastPos_]);
  }
  return nullptr;
}

template < typename ElementType, cSize Capacity >
ElementType* at(FixedDEQueue< ElementType, Capacity >& container, cSize pos)
{
  if(pos < container.length_)
  {
    cSize index = (pos + container.firstPos_) % container.maxLength;
    return &(container.array_[index]);
  }
  return nullptr;
}

template < typename ElementType, cSize Capacity >
void clear(FixedDEQueue< ElementType, Capacity >& container)
{
  container.length_ = 0;
  container.firstPos_ = Capacity >> 1;
  container.lastPos_ = container.firstPos_;
}

template < typename ElementType, cSize Capacity >
cSize length(FixedDEQueue< ElementType, Capacity >& container)
{
  return container.length_;
}

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_CONTAINERS_HPP
