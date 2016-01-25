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

// forward declarations
template < typename ElementType, cSize Capacity >
struct FixedDeQueue;

template < typename ElementType, cSize Capacity >
ErrorCode push_back(FixedDeQueue< ElementType, Capacity >& container, const ElementType& el);

template < typename ElementType, cSize Capacity >
ErrorCode push_front(FixedDeQueue< ElementType, Capacity >& container, const ElementType& el);

template < typename ElementType, cSize Capacity >
ElementType pop_back(FixedDeQueue< ElementType, Capacity >& container, ElementType fallback);

template < typename ElementType, cSize Capacity >
ElementType pop_front(FixedDeQueue< ElementType, Capacity >& container, ElementType fallback);

template < typename ElementType, cSize Capacity >
ElementType* front(FixedDeQueue< ElementType, Capacity >& container);

template < typename ElementType, cSize Capacity >
ElementType* back(FixedDeQueue< ElementType, Capacity >& container);

template < typename ElementType, cSize Capacity >
ElementType* at(FixedDeQueue< ElementType, Capacity >& container, cSize pos);

template < typename ElementType, cSize Capacity >
ErrorCode del(FixedDeQueue< ElementType, Capacity >& container, cSize pos);

template < typename ElementType, cSize Capacity >
cSize length(FixedDeQueue< ElementType, Capacity >& container);

// declarations
template < typename ElementType, cSize Capacity >
struct FixedDeQueue
{
  using Type = ElementType;
  const cSize maxLength = Capacity;

  FixedDeQueue() = delete;
  explicit FixedDeQueue(const ElementType& init);
  FixedDeQueue(const FixedDeQueue& other);
  FixedDeQueue& operator=(const FixedDeQueue& other);

private:
  Type array_[Capacity];
  cSize length_;
  cSize internalLength_;
  cSize firstPos_;
  cSize lastPos_;

  friend ErrorCode push_back< ElementType, Capacity >(FixedDeQueue& container,
                                                      const ElementType& el);

  friend ErrorCode push_front< ElementType, Capacity >(FixedDeQueue& container,
                                                       const ElementType& el);

  friend ElementType pop_back< ElementType, Capacity >(FixedDeQueue& container,
                                                       ElementType fallback);

  friend ElementType pop_front< ElementType, Capacity >(FixedDeQueue& container,
                                                        ElementType fallback);

  friend ElementType* front< ElementType, Capacity >(FixedDeQueue& container);

  friend ElementType* back< ElementType, Capacity >(FixedDeQueue& container);

  friend ElementType* at< ElementType, Capacity >(FixedDeQueue& container, cSize pos);

  friend ErrorCode del< ElementType, Capacity >(FixedDeQueue& container, cSize pos);

  friend cSize length< ElementType, Capacity >(FixedDeQueue& container);
};

template < typename ElementType, cSize Capacity >
FixedDeQueue< ElementType, Capacity >::FixedDeQueue(const ElementType& init)
    : array_{}
    , length_{0}
    , internalLength_{0}
    , firstPos_{Capacity >> 1}
    , lastPos_{Capacity >> 1}
{
  std::fill(array_, array_ + Capacity, init);
}

template < typename ElementType, cSize Capacity >
FixedDeQueue< ElementType, Capacity >::FixedDeQueue(const FixedDeQueue& other)
    : array_{}
    , length_{other.length}
    , internalLength_{other.nternalLentgth}
    , firstPos_{other.firstPos}
    , lastPos_{other.lastPos}
{
  std::copy(other.array_, other.array_ + other.internalLength, array_);
}

template < typename ElementType, cSize Capacity >
FixedDeQueue< ElementType, Capacity >& FixedDeQueue< ElementType, Capacity >::
operator=(const FixedDeQueue& other)
{
  length_ = other.length;
  internalLength_ = other.internalLentgth;
  firstPos_ = other.firstPos;
  lastPos_ = other.lastPos;
  std::copy(other.array_, other.array_ + other.internalLength, array_);
  return *this;
}

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_CONTAINERS_HPP
