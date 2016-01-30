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
template < typename ElementType, cSize Capacity, bool canOverwrite = false >
struct FixedArray
{
  using Type = ElementType;
  GLOBAL const cSize maxLength_;
  GLOBAL const bool canOverwrite_;

  cSize firstPos_;
  cSize lastPos_;
  cSize length_;
  Type array_[Capacity];

  FixedArray() = delete;
  explicit FixedArray(const ElementType& init);
  FixedArray(const FixedArray& other);
  // TODO: Move constructor???
  FixedArray& operator=(const FixedArray& other);
};

// GLOBAL
template < typename ElementType, cSize Capacity, bool canOverwrite >
const cSize FixedArray< ElementType, Capacity, canOverwrite >::maxLength_{Capacity};
template < typename ElementType, cSize Capacity, bool canOverwrite >
const bool FixedArray< ElementType, Capacity, canOverwrite >::canOverwrite_{canOverwrite};

// implements the constructors
template < typename ElementType, cSize Capacity, bool canOverwrite >
FixedArray< ElementType, Capacity, canOverwrite >::FixedArray(const ElementType& init)
    : firstPos_{0}
    , lastPos_{Capacity}
    , length_{Capacity}
    , array_{}
{
  std::fill(array_, (array_ + maxLength_), init);
}

template < typename ElementType, cSize Capacity, bool canOverwrite >
FixedArray< ElementType, Capacity, canOverwrite >::FixedArray(const FixedArray& other)
    : firstPos_{other.firstPos_}
    , lastPos_{other.lastPos_}
    , length_{other.length_}
    , array_{}
{
  std::copy(other.array_, (other.array_ + std::min(other.maxLength_, maxLength_)), array_);
}

template < typename ElementType, cSize Capacity, bool canOverwrite >
FixedArray< ElementType, Capacity, canOverwrite >&
FixedArray< ElementType, Capacity, canOverwrite >::operator=(const FixedArray& other)
{
  std::copy(
      other.array_, (other.array_ + std::min(other.maxLength_, this->maxLength_)), this->array_);
  this->firstPos_ = other.firstPos_;
  this->lastPos_ = other.lastPot_;
  this->length_ = std::min(other.length_, this->length_);
  return *this;
}

// accessor methods
template < typename ElementType, cSize Capacity, bool canOverwrite >
inline ElementType* at(FixedArray< ElementType, Capacity, canOverwrite >& container, cSize pos)
{
  if(pos < container.length_)
  {
    return &(container.array_[(pos + container.firstPos_) % container.maxLength_]);
  }
  return nullptr;
}

template < typename ElementType, cSize Capacity, bool canOverwrite >
inline cSize len(FixedArray< ElementType, Capacity, canOverwrite >& container)
{
  return container.length_;
}

// this needs to be reimplemente on the other containers because of the resting the cursors
template < typename ElementType, cSize Capacity, bool canOverwrite >
inline void clear(FixedArray< ElementType, Capacity, canOverwrite >& container,
                  const ElementType& init)
{
  std::fill(container.array_, (container.array_ + container.maxLength_), init);
  container.firstPos_ = 0;
  container.lastPos_ = container.maxLength_;
  container.length_ = container.maxLength_;
}

///////////////////////////////////////////////////////////////////////////////
// Stack : Fixed size Stack with random accessor
///////////////////////////////////////////////////////////////////////////////

// declarations
template < typename ElementType, cSize Capacity, bool canOverwrite = false >
struct FixedStack : public FixedArray< ElementType, Capacity, canOverwrite >
{
  FixedStack() = delete;
  explicit FixedStack(const ElementType& init);
  FixedStack(const FixedStack& other);
  // TODO: Move constructor???
  FixedStack& operator=(const FixedStack& other);
};

// implements the constructors
template < typename ElementType, cSize Capacity, bool canOverwrite >
FixedStack< ElementType, Capacity, canOverwrite >::FixedStack(const ElementType& init)
    : FixedArray< ElementType, Capacity, canOverwrite >(init)
{
  this->lastPos_ = 0;
  this->length_ = 0;
}

template < typename ElementType, cSize Capacity, bool canOverwrite >
FixedStack< ElementType, Capacity, canOverwrite >::FixedStack(const FixedStack& other)
    : FixedArray< ElementType, Capacity, canOverwrite >(other)
{
}

template < typename ElementType, cSize Capacity, bool canOverwrite >
FixedStack< ElementType, Capacity, canOverwrite >&
FixedStack< ElementType, Capacity, canOverwrite >::operator=(const FixedStack& other)
{
  std::copy(
      other.array_, (other.array_ + std::min(other.maxLength_, this->maxLength_)), this->array_);
  this->length_ = other.length_;
  this->firstPos_ = other.firstPos_;
  this->lastPos_ = other.lastPot_;
  return *this;
}

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

// reimplemented due to different cursor resting values
template < typename ElementType, cSize Capacity, bool canOverwrite >
inline void clear(FixedStack< ElementType, Capacity, canOverwrite >& container,
                  const ElementType& init)
{
  std::fill(container.array_, (container.array_ + container.maxLength_), init);
  container.length_ = 0;
  container.firstPos_ = 0;
  container.lastPos_ = 0;
}

///////////////////////////////////////////////////////////////////////////////
// DEQueue : fixex size double ended queue with random accessor
///////////////////////////////////////////////////////////////////////////////

// declarations
template < typename ElementType, cSize Capacity, bool canOverwrite = false >
struct FixedDEQ : public FixedStack< ElementType, Capacity, canOverwrite >
{
  FixedDEQ() = delete;
  explicit FixedDEQ(const ElementType& init);
  FixedDEQ(const FixedDEQ& other);
  // TODO: Move constructor???
  FixedDEQ& operator=(const FixedDEQ& other);
};

// implements the constructors
template < typename ElementType, cSize Capacity, bool canOverwrite >
FixedDEQ< ElementType, Capacity, canOverwrite >::FixedDEQ(const ElementType& init)
    : FixedStack< ElementType, Capacity, canOverwrite >(init)
{
  this->length_ = 0;
  this->firstPos_ = Capacity >> 1;
  this->lastPos_ = Capacity >> 1;
}

template < typename ElementType, cSize Capacity, bool canOverwrite >
FixedDEQ< ElementType, Capacity, canOverwrite >::FixedDEQ(const FixedDEQ& other)
    : FixedStack< ElementType, Capacity, canOverwrite >(other)
{
}

template < typename ElementType, cSize Capacity, bool canOverwrite >
FixedDEQ< ElementType, Capacity, canOverwrite >& FixedDEQ< ElementType, Capacity, canOverwrite >::
operator=(const FixedDEQ& other)
{
  std::copy(
      other.array_, (other.array_ + std::min(other.maxLength_, this->maxLength_)), this->array_);
  this->length_ = other.length;
  this->firstPos_ = other.firstPos;
  this->lastPos_ = other.lastPos;
  return *this;
}

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

template < typename ElementType, cSize Capacity, bool canOverwrite >
inline void clear(FixedDEQ< ElementType, Capacity, canOverwrite >& container,
                  const ElementType& init)
{
  std::fill(container.array_, (container.array_ + container.maxLength_), init);
  container.length_ = 0;
  container.firstPos_ = Capacity >> 1;
  container.lastPos_ = container.firstPos_;
}

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_FIXCONTAINERS_HPP
