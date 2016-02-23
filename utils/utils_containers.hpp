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
  Type init_;

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

///////////////////////////////////////////////////////////////////////////////
// List
///////////////////////////////////////////////////////////////////////////////
template < typename Type >
struct ListInterface
{
  virtual cSize capacity() = 0;
  virtual cSize initialLen() = 0;

  struct ElementType
  {
    Type data;
    ElementType* next;
    ElementType* prev;
  };

  cSize length_;
  ElementType* head_;
  ElementType* tail_;
  ElementType* free_;

  virtual ~ListInterface(){};
  ListInterface();
  explicit ListInterface(ListInterface& other);
};

template < typename Type >
ListInterface< Type >::ListInterface()
    : length_{0}
    , head_{nullptr}
    , tail_{nullptr}
    , free_{nullptr}
{
}

template < typename Type >
ListInterface< Type >::ListInterface(ListInterface& other)
    : length_{0}
    , head_{nullptr}
    , tail_{nullptr}
    , free_{nullptr}
{
}

///////////////////////////////////////////////////////////////////////////////
// Acessors
///////////////////////////////////////////////////////////////////////////////
// FIXME: change at, front, back, pop and push to match STL interface.
// at, front and bacck return a reference
// pop and push just remove the items and return nothing

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
inline void clear(ArrayInterface< Type >& container)
{
  std::fill(container.array_, (container.array_ + container.capacity()), container.init);
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

// List
template < typename Type >
inline cSize len(ListInterface< Type >& container)
{
  return container.length_;
}

template < typename Type >
inline void clear(ListInterface< Type >& container)
{
  container.length_ = 0;
  if(container.head_)
  {
    if(!container.free_)
    {
      container.free_ = container.head_;
      container.head_ = nullptr;
    }
    else
    {
      container.free_->next = container.head_;
      container.head_->prev = container.free_;
      container.head_ = nullptr;
    }
  }
  container.tail_ = nullptr;
}

template < typename Type >
inline void push_front(ListInterface< Type >& container, const Type& element)
{
  typename ListInterface< Type >::ElementType* newElement = container.free_;
  container.free_ = container.free_->next;
  newElement->data = element;
  newElement->next = container.head_;
  newElement->prev = nullptr;
  container.head_->pre = newElement;
  container.head_ = newElement;

  ++container.length_;
}

template < typename Type >
inline void push_back(ListInterface< Type >& container, const Type& element)
{
  typename ListInterface< Type >::ElementType* newElement = container.free_;
  container.free_ = container.free_->next;
  newElement->data = element;
  newElement->next = nullptr;
  newElement->prev = container.tail_;
  container.tail_->next = newElement;
  container.tail_ = newElement;

  ++container.length_;
}

template < typename Type >
inline Type pop_front(ListInterface< Type >& container)
{
  typename ListInterface< Type >::ElementType* iterator = container.head_;

  container.head_ = iterator->next;
  container.free_->prev = iterator;
  iterator->next = container.free_;
  iterator->prev = nullptr;
  container.free = iterator;

  --container.length_;
  return iterator->data;
}

template < typename Type >
inline Type pop_back(ListInterface< Type >& container)
{
  typename ListInterface< Type >::ElementType* iterator = container.tail_;

  container.tail_ = iterator->prev;
  container.free_->prev = iterator;
  iterator->next = container.free_;
  iterator->prev = nullptr;
  container.free = iterator;

  --container.length_;
  return iterator->data;
}

template < typename Type >
inline bool insert(ListInterface< Type >& container,
                   const typename ListInterface< Type >::ElementType* cursor,
                   const Type& element)
{
  typename ListInterface< Type >::ElementType* iterator = container.head_;
  do
  {
    if(iterator == element)
    {
      typename ListInterface< Type >::ElementType* newElement = container.free_;
      container.free_ = container.free_->next;
      newElement->data = element;
      newElement->next = iterator->next;
      newElement->prev = iterator;
      iterator->next = newElement;
      ++container.length_;

      return true;
    }
    iterator = iterator->next;
  } while(iterator != nullptr);

  return false;
}

template < typename Type >
inline bool remove(ListInterface< Type >& container,
                   const typename ListInterface< Type >::ElementType* element)
{
  typename ListInterface< Type >::ElementType* iterator = container.head_;
  if(iterator == element)
  {
    container.head_ = iterator->next;
    container.free_->prev = iterator;
    iterator->next = container.free_;
    iterator->prev = nullptr;
    container.free = iterator;
    --container.length_;

    return true;
  }
  else
  {
    do
    {
      iterator = iterator->next;
      if(iterator == element)
      {
        iterator->prev->next = iterator->next;
        iterator->next->prev = iterator->prev;
        container.free_->prev = iterator;
        iterator->next = container.free_;
        iterator->prev = nullptr;
        container.free = iterator;
        --container.length_;

        return true;
      }
    } while(iterator != container.tail);
  }
  return false;
}
// TODO: missing front and back

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_CONTAINERS_HPP
