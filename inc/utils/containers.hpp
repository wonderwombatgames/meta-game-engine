/**
  * basic data container interfaces
  *
  */

#ifndef UTILS_CONTAINERS_HPP
#define UTILS_CONTAINERS_HPP

#include <algorithm>

#include "utils/types.hpp"

namespace W2E
{

namespace Utils
{

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

// default constructor
template < typename Type >
ArrayInterface< Type >::ArrayInterface()
    : firstPos_{0}
    , lastPos_{0}
    , length_{0}
    , array_{nullptr}
    , init_{}
{
}

// copy constructor
template < typename Type >
ArrayInterface< Type >::ArrayInterface(ArrayInterface& other)
    : firstPos_{other.firstPos}
    , lastPos_{other.lastPos_}
    , length_{other.length_}
    , array_{nullptr}
    , init_{}
{
}

// assignement operator
template < typename Type >
ArrayInterface< Type >& ArrayInterface< Type >::operator=(const ArrayInterface< Type >& other)
{
  this->firstPos_ = other.firstPos;
  this->lastPos_ = other.lastPos_;
  this->length_ = other.length_;
  this->array_ = nullptr;
  this->init_ = other.init_;
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

// default constructor
template < typename Type >
DEQInterface< Type >::DEQInterface()
    : ArrayInterface< Type >()
    , canOverwrite_{false}
{
}

// copy constructor
template < typename Type >
DEQInterface< Type >::DEQInterface(DEQInterface< Type >& other)
    : ArrayInterface< Type >(other)
    , canOverwrite_{other.canOverwrite_}
{
}

// assgignement operator
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
  ListInterface& operator=(const ListInterface& other);
};

// default constructor
template < typename Type >
ListInterface< Type >::ListInterface()
    : length_{0}
    , head_{nullptr}
    , tail_{nullptr}
    , free_{nullptr}
{
}

// copy constructor
template < typename Type >
ListInterface< Type >::ListInterface(ListInterface& other)
    : length_{0}
    , head_{nullptr}
    , tail_{nullptr}
    , free_{nullptr}
{
}

// assignement operator
template < typename Type >
ListInterface< Type >& ListInterface< Type >::operator=(ListInterface< Type >& other)
{
  // FIXME: deep copy
  // this->length_ = other.length_;
  // this->head_   = other.head_;
  // this->tail_   = other.tail_;
  // this->free_   = other.free_;
}

///////////////////////////////////////////////////////////////////////////////
// Tree
///////////////////////////////////////////////////////////////////////////////

template < typename Type >
struct TreeInterface // : Array ???should this inherite from Array???
{
  enum NodeColour
  {
    Red,
    Black,
  };

  struct ElementType
  {
    Type data;
    NodeColour colour;
    ElementType* parent;
    ElementType* left;
    ElementType* right;
  };

  cSize length_;
  ElementType* root_;
  ElementType* free_;

  virtual ~TreeInterface(){};
  TreeInterface();
  explicit TreeInterface(const TreeInterface& other);
  TreeInterface& operator=(const TreeInterface& other);
  // TODO: Move constructor???
};

///////////////////////////////////////////////////////////////////////////////
// Trie
///////////////////////////////////////////////////////////////////////////////

template < typename Type >
struct TrieInterface
{
  virtual ~TrieInterface() {}
  TrieInterface();
  explicit TrieInterface(const TrieInterface& other);
  TrieInterface& operator=(const TrieInterface& other);
  // TODO: Move constructor???
};

///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

// Array acessor functions
// ----------------------------------------------------------------------------

// random accessor
// @param container container to access
// @param pos       position to access
// return pointer to the element at the container required position
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

// length of the container
// @param   container
// @return  size
template < typename Type >
inline cSize len(ArrayInterface< Type >& container)
{
  return container.length_;
}

// clear the container
// @param container
template < typename Type >
inline void clear(ArrayInterface< Type >& container)
{
  std::fill(container.array_, (container.array_ + container.capacity()), container.init);
  container.firstPos_ = 0;
  container.lastPos_ = container.initialLen();
  container.length_ = container.initialLen();
}

// DEQ accessor functions
// ----------------------------------------------------------------------------

// push the element to the back of the container (copy the content)
// NOTE: the element has to be copyable!!!
// @param container
// @param el  element to be copied at the container
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode push_back(DEQInterface< Type >& container, const Type& el)
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

// push the element to the front of the container (copy the content)
// NOTE: the element has to be copyable!!!
// @param container
// @param el  element to be copied at the container
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
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

// pop the element from the back of the container.
// NOTE: the element will be destroyed - pointers to this element will be lost
// @param container
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode pop_back(DEQInterface< Type >& container)
{
  m64 mm(container.capacity(), 0);
  if(container.length_ > 0)
  {
    cSize previousPos = container.lastPos_;
    mm = container.length_;
    container.length_ = (--mm).toInt();
    mm = container.lastPos_;
    container.lastPos_ = (--mm).toInt();

    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

// pop the element from the front of the container.
// NOTE: the element will be destroyed - pointers to this element will be lost
// @param container
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode pop_front(DEQInterface< Type >& container)
{
  m64 mm(container.capacity(), 0);
  if(container.length_ > 0)
  {
    cSize previousPos = container.firstPos_;

    mm = container.length_;
    container.length_ = (--mm).toInt();

    mm = container.firstPos_;
    container.firstPos_ = (++mm).toInt();

    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

// get the element from the front of the container.
// @param container
// return pointer to the element at the front of the container
template < typename Type >
inline Type* front(DEQInterface< Type >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[container.firstPos_]);
  }
  return nullptr;
}

// get the element from the back of the container.
// @param container
// return pointer to the element at the front of the container
template < typename Type >
inline Type* back(DEQInterface< Type >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[container.lastPos_]);
  }
  return nullptr;
}

// List accessor functions
// ----------------------------------------------------------------------------

// random accessor
// @param container container to access
// @param pos       position to access
// return pointer to the element at the container required position
template < typename Type >
inline typename ListInterface< Type >::ElementType* at(ListInterface< Type >& container, cSize pos)
{
  if(container.head_ && container.Length_ >= pos)
  {
    // search for element in the position
    // FIXME: if pos > (container.Length_/2) then better to search backwards
    typename ListInterface< Type >::ElementType* iterator = container.head_;
    cSize cursor = 0;
    do
    {
      if(cursor == pos)
      {
        return iterator;
      }
      else
      {
        iterator = iterator->next;
        ++cursor;
      }
    } while(iterator != nullptr && cursor < pos);
  }
  return nullptr;
}

// length of the container
// @param   container
// @return  size
template < typename Type >
inline cSize len(ListInterface< Type >& container)
{
  return container.length_;
}

// clear the container
// @param container
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

// push the element to the front of the container (copy the content)
// NOTE: the element has to be copyable!!!
// @param container
// @param el  element to be copied at the container
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode push_front(ListInterface< Type >& container, const Type& element)
{
  if(container.free_)
  {
    typename ListInterface< Type >::ElementType* newElement = container.free_;
    container.free_ = container.free_->next;
    newElement->data = element;
    newElement->next = container.head_;
    newElement->prev = nullptr;
    container.head_->prev = newElement;
    container.head_ = newElement;

    ++container.length_;

    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

// push the element to the back of the container (copy the content)
// NOTE: the element has to be copyable!!!
// @param container
// @param el  element to be copied at the container
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode push_back(ListInterface< Type >& container, const Type& element)
{
  if(container.free_)
  {
    typename ListInterface< Type >::ElementType* newElement = container.free_;
    container.free_ = container.free_->next;
    newElement->data = element;
    newElement->next = nullptr;
    newElement->prev = container.tail_;
    container.tail_->next = newElement;
    container.tail_ = newElement;

    ++container.length_;

    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

// pop the element from the front of the container.
// NOTE: the element will be destroyed - pointers to this element will be lost
// @param container
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode pop_front(ListInterface< Type >& container)
{
  if(container.head_)
  {
    typename ListInterface< Type >::ElementType* iterator = container.head_;

    container.head_ = iterator->next;
    container.free_->prev = iterator;
    iterator->next = container.free_;
    iterator->prev = nullptr;
    container.free = iterator;

    --container.length_;

    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

// pop the element from the back of the container.
// NOTE: the element will be destroyed - pointers to this element will be lost
// @param container
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode pop_back(ListInterface< Type >& container)
{
  if(container.tail_)
  {
    typename ListInterface< Type >::ElementType* iterator = container.tail_;

    container.tail_ = iterator->prev;
    container.free_->prev = iterator;
    iterator->next = container.free_;
    iterator->prev = nullptr;
    container.free = iterator;

    --container.length_;

    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

// get the element from the front of the container.
// @param container
// return pointer to the element at the front of the container
template < typename Type >
inline typename ListInterface< Type >::ElementType* front(ListInterface< Type >& container)
{
  return container.head_;
}

// get the element from the back of the container.
// @param container
// return pointer to the element at the front of the container
template < typename Type >
inline typename ListInterface< Type >::ElementType* back(ListInterface< Type >& container)
{
  return container.tail_;
}

// insert one element from the container.
// NOTE: the element has to be copyable!!!
// @param container
// @param element element to be inserted (will be copied)
// @param postion to include the element
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode insert(ListInterface< Type >& container,
                        const typename ListInterface< Type >::ElementType& element,
                        cSize pos)
{
  if(container.free_ && container.head_ && (container.Length_ >= pos))
  {
    // search for element in the position
    // FIXME: if pos > (container.Length_/2) then better to search backwards
    typename ListInterface< Type >::ElementType* iterator = container.head_;
    cSize cursor = 0;
    while(cursor < pos && iterator)
    {
      iterator = iterator->next;
      ++cursor;
    }

    if(iterator)
    {
      typename ListInterface< Type >::ElementType* newElement = container.free_;
      container.free_ = container.free_->next;
      newElement->data = element;
      newElement->next = iterator;
      newElement->prev = iterator->prev;
      iterator->prev = newElement;
      newElement->prev->next = newElement;

      ++container.length_;

      return NO_ERROR;
    }
  }
  return UNKNOWN_ERROR;
}

// remove one element from the container.
// NOTE: the element to be removed has to be a pointer to the position
// @param container
// @param element pointer to the element to be removed
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode remove(ListInterface< Type >& container,
                        const typename ListInterface< Type >::ElementType* element)
{
  if(container.head_)
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

      return NO_ERROR;
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

          return NO_ERROR;
        }
      } while(iterator != container.tail);
    }
  }
  return UNKNOWN_ERROR;
}

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_CONTAINERS_HPP
