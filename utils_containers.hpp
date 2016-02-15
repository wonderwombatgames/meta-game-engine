/**
  *
  */

#ifndef UTILS_CONTAINERS_HPP
#define UTILS_CONTAINERS_HPP

#include <algorithm>

#include "utils_types.hpp"

namespace W2E
{

namespace Utils
{

// declarations

///////////////////////////////////////////////////////////////////////////////
// Array
///////////////////////////////////////////////////////////////////////////////

template < typename Type >
struct ArrayInterface
{
  using ElementType = Type;

  virtual cSize capacity() = 0;
  virtual cSize initialLen() = 0;

  cSize firstPos_;
  cSize lastPos_;
  cSize length_;
  Type* array_;

  virtual ~ArrayInterface() {}
  ArrayInterface();
  explicit ArrayInterface(ArrayInterface& other);
  ArrayInterface& operator=(const ArrayInterface& other);
};

template < typename Type >
ArrayInterface< Type >::ArrayInterface()
    : firstPos_{0}
    , lastPos_{0}
    , length_{0}
    , array_{nullptr}
{
}

template < typename Type >
ArrayInterface< Type >::ArrayInterface(ArrayInterface& other)
    : firstPos_{other.firstPos}
    , lastPos_{other.lastPos_}
    , length_{other.length_}
    , array_{nullptr}
{
}

template < typename Type >
ArrayInterface< Type >& ArrayInterface< Type >::operator=(const ArrayInterface< Type >& other)
{
  this->firstPos_ = other.firstPos;
  this->lastPos_ = other.lastPos_;
  this->length_ = other.length_;
  this->array_ = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
// DEQ
///////////////////////////////////////////////////////////////////////////////

template < typename Type >
struct DEQInterface : ArrayInterface< Type >
{
  bool canOverwrite_;

  virtual ~DEQInterface() {}
  DEQInterface();
  explicit DEQInterface(DEQInterface& other);
  DEQInterface& operator=(DEQInterface& other);
};

template < typename Type >
DEQInterface< Type >::DEQInterface()
    : ArrayInterface< Type >()
    , canOverwrite_{false}
{
}

template < typename Type >
DEQInterface< Type >::DEQInterface(DEQInterface< Type >& other)
    : ArrayInterface< Type >(other)
    , canOverwrite_{other.canOverwrite_}
{
}

template < typename Type >
DEQInterface< Type >& DEQInterface< Type >::operator=(DEQInterface< Type >& other)
{
  this->canOverwrite_ = other.canOverwrite_;
}

// TODO: Need List???

///////////////////////////////////////////////////////////////////////////////
// Acessors
///////////////////////////////////////////////////////////////////////////////

// Array
template < typename Type >
inline Type* at(ArrayInterface< Type >& container, cSize pos)
{
  m64 logicPos(container.capacity(), pos);
  if(logicPos < container.length_)
  {
    logicPos += container.firstPos_;
    return (container.array_ + logicPos.toInt());
  }
  return nullptr;
}

template < typename Type >
inline cSize len(ArrayInterface< Type >& container)
{
  return container.length_;
}

template < typename Type >
inline void clear(ArrayInterface< Type >& container, const Type& init)
{
  std::fill(container.array_, (container.array_ + container.capacity()), init);
  container.firstPos_ = 0;
  container.lastPos_ = container.initialLen();
  container.length_ = container.initialLen();
}

// DEQ
template < typename Type >
inline ErrorCode push(DEQInterface< Type >& container, const Type& el)
{
  if((container.length_ < container.capacity()) || container.canOverwrite_)
  {
    m64 mm(container.capacity(), 0);
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
    container.length_ = std::min(currLen, container.capacity());
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

template < typename Type >
inline Type pop(DEQInterface< Type >& container, Type fallback)
{
  m64 mm(container.capacity(), 0);
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

template < typename Type >
inline Type* bot(DEQInterface< Type >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[container.firstPos_]);
  }
  return nullptr;
}

template < typename Type >
inline Type* top(DEQInterface< Type >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[container.lastPos_]);
  }
  return nullptr;
}

template < typename Type >
inline ErrorCode push_back(DEQInterface< Type >& container, const Type& el)
{
  return push(container, el);
}

template < typename Type >
inline ErrorCode push_front(DEQInterface< Type >& container, const Type& el)
{
  m64 mm(container.capacity(), 0);
  if((container.length_ < container.capacity()) || container.canOverwrite_)
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
    container.length_ = std::min(currLen, container.capacity());
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

template < typename Type >
inline Type pop_back(DEQInterface< Type >& container, Type fallback)
{
  return pop(container, fallback);
}

template < typename Type >
inline Type pop_front(DEQInterface< Type >& container, Type fallback)
{
  m64 mm(container.capacity(), 0);
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

template < typename Type >
inline Type* front(DEQInterface< Type >& container)
{
  return bot(container);
}

template < typename Type >
inline Type* back(DEQInterface< Type >& container)
{
  return top(container);
}

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_CONTAINERS_HPP
