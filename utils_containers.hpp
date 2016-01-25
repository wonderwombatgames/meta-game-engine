/**
  * helper functions to abstrct the backend
  *
  */

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
ErrorCode erase(FixedDeQueue< ElementType, Capacity >& container, cSize pos);

template < typename ElementType, cSize Capacity >
cSize length(FixedDeQueue< ElementType, Capacity >& container);

// declarations
template < typename ElementType, cSize Capacity >
struct FixedDeQueue
{
  using Type = ElementType;
  const cSize maxLength = Capacity;

  FixedDeQueue();
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

  friend ErrorCode erase< ElementType, Capacity >(FixedDeQueue& container, cSize pos);

  friend cSize length< ElementType, Capacity >(FixedDeQueue& container);
};

template < typename ElementType, cSize Capacity >
FixedDeQueue< ElementType, Capacity >::FixedDeQueue()
    : array_{}
    , length_{0}
    , internalLength_{0}
    , firstPos_{0}
    , lastPos_{0}
{
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
