/**
  * helper functions to abstrct the backend
  *
  */

#ifndef UTILS_STRINGS_HPP
#define UTILS_STRINGS_HPP

#include <algorithm>
#include <cstring>
// FIXME: have to try to remove this... replace for struct
#include <tuple>

#include "utils/types.hpp"

namespace // anonymous
{
using namespace W2E::Utils;

// http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogObvious
cSize alignMem(const cSize& desired)
{
  cSize v = desired;
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return v;
}

} // end namespace anonymous

namespace W2E
{

namespace Utils
{

///////////////////////////////////////////////////////////////////////////////
// String : Fixed size String pool
///////////////////////////////////////////////////////////////////////////////

// forward declarations
class StringWrapper;

class StringPoolInterface
{
public:
  virtual ~StringPoolInterface() {}

private:
  friend class StringWrapper;
  virtual std::tuple< ErrorCode, char*, u16* > allocate(const u16& length) = 0;
  virtual ErrorCode deallocate(const u16& length, char* str, u16* refCount) = 0;
};

// TODO: implemente using the new allocators!
template < cSize Capacity >
class DynStringPool : public StringPoolInterface
{
public:
  virtual ~DynStringPool() {}
  DynStringPool();

private:
  virtual std::tuple< ErrorCode, char*, u16* > allocate(const u16& length) override;
  virtual ErrorCode deallocate(const u16& length, char* str, u16* refCount) override;
  DynStringPool(DynStringPool&) = delete;
  DynStringPool& operator=(DynStringPool&) = delete;
};

template < cSize Capacity >
class FixedStringPool : public StringPoolInterface
{
public:
  virtual ~FixedStringPool() {}
  FixedStringPool();

private:
  virtual std::tuple< ErrorCode, char*, u16* > allocate(const u16& length) override;
  virtual ErrorCode deallocate(const u16& length, char* str, u16* refCount) override;
  FixedStringPool(FixedStringPool&) = delete;
  FixedStringPool& operator=(FixedStringPool&) = delete;

  // data
  GLOBAL const cSize maxLength_{Capacity};
  GLOBAL const cSize maxElements_{maxLength_ >> 3}; // smallest string is 2^3 chars

  GLOBAL char array_[maxLength_];
  GLOBAL cSize arrayCursor_;

  GLOBAL u16 refCount_[maxElements_];
  GLOBAL cSize refCountCursor_;
};

template < cSize Capacity >
char FixedStringPool< Capacity >::array_[maxLength_] = {0};
template < cSize Capacity >
cSize FixedStringPool< Capacity >::arrayCursor_ = 0;
template < cSize Capacity >
u16 FixedStringPool< Capacity >::refCount_[maxElements_] = {0};
template < cSize Capacity >
cSize FixedStringPool< Capacity >::refCountCursor_ = 0;

template < cSize Capacity >
FixedStringPool< Capacity >::FixedStringPool()
{
}

template < cSize Capacity >
std::tuple< ErrorCode, char*, u16* > FixedStringPool< Capacity >::allocate(const u16& length)
{

  cSize len = std::max(static_cast< cSize >(8), alignMem(length));
  if((this->maxLength_ - this->arrayCursor_) > len && (this->maxElements_ > this->refCountCursor_))
  {
    // set the refCount
    u16 mapLen = len >> 3;
    u16* refCount = &(this->refCount_[this->refCountCursor_]);
    std::fill(refCount, (refCount + mapLen), 0);
    this->refCountCursor_ += mapLen;

    // grab the string pointer and clear it
    char* str = &(this->array_[this->arrayCursor_]);
    this->arrayCursor_ += len;
    std::fill(str, (str + len), 0);

    return std::make_tuple(NO_ERROR, str, refCount);
  }
  else
  {
    // search the refCount array and reuse freed memory
    // FIXME: this has to use other kind of allocation later!!!
    u16 mapLen = len >> 3;
    u16* pattern = new u16[mapLen];
    std::fill(pattern, (pattern + mapLen), 0);
    u16* refCount = std::search(
        this->refCount_, (this->refCount_ + this->refCountCursor_), pattern, (pattern + mapLen));
    delete[] pattern;

    if(refCount < (this->refCount_ + this->refCountCursor_))
    {
      u16 arrayCur = (refCount - this->refCount_) << 3;
      char* str = this->array_ + arrayCur;
      return std::make_tuple(NO_ERROR, str, refCount);
    }
  }
  return std::make_tuple(UNKNOWN_ERROR, nullptr, nullptr);
}

template < cSize Capacity >
ErrorCode FixedStringPool< Capacity >::deallocate(const u16& length, char* str, u16* refCount)
{
  cSize len = std::max(static_cast< cSize >(8), alignMem(length));
  u16 mapLen = len >> 3;

  // clear refCount
  std::fill(refCount, (refCount + mapLen), 0);

  // clear string pool segment
  std::fill(str, (str + len), 0);

  return NO_ERROR;
}

class StringWrapper
{
public:
  ~StringWrapper();
  explicit StringWrapper(StringPoolInterface* sp);
  explicit StringWrapper(StringPoolInterface* sp, const char* initStr);
  StringWrapper(StringWrapper& otherStr);
  // TODO: move constructor???
  StringWrapper& operator=(StringWrapper& otherStr);
  StringWrapper& operator=(const char* initStr); // write on change
  const char operator[](u16 pos) const;
  char operator[](u16 pos); // write on change
  const char* cStr();

private:
  // data
  StringPoolInterface* strPool_;
  u16 length_;
  u16* refCount_;
  char* str_;
};

StringWrapper::~StringWrapper()
{
  this->strPool_->deallocate(this->length_, this->str_, this->refCount_);
}

StringWrapper::StringWrapper(StringPoolInterface* sp, const char* initStr)
    : strPool_{sp}
    , length_{0}
    , refCount_{nullptr}
    , str_{nullptr}
{
  u16 inLen = std::strlen(initStr);
  u16 len = std::max(static_cast< cSize >(8), alignMem(inLen));
  auto ret = this->strPool_->allocate(len);
  if(std::get< 0 >(ret) == NO_ERROR)
  {
    this->str_ = std::get< 1 >(ret);
    this->refCount_ = std::get< 2 >(ret);

    std::copy_n(initStr, inLen, this->str_);
  }
}

StringWrapper::StringWrapper(StringPoolInterface* sp)
    : strPool_{sp}
    , length_{0}
    , refCount_{nullptr}
    , str_{nullptr}
{
}

StringWrapper::StringWrapper(StringWrapper& otherStr)
    : strPool_{otherStr.strPool_}
    , length_{otherStr.length_}
    , refCount_{otherStr.refCount_}
    , str_{otherStr.str_}
{
  ++(*refCount_);
}

StringWrapper& StringWrapper::operator=(StringWrapper& otherStr)
{
  this->strPool_ = otherStr.strPool_;
  this->length_ = otherStr.length_;
  this->str_ = otherStr.str_;

  this->refCount_ = otherStr.refCount_;
  ++(*refCount_);

  return *this;
}

StringWrapper& StringWrapper::operator=(const char* initStr)
{
  if((*this->refCount_) > 1)
  {
    this->refCount_ -= 1;
  }

  u16 inLen = std::strlen(initStr);
  u16 len = std::max(static_cast< cSize >(8), alignMem(inLen));
  auto ret = this->strPool_->allocate(len);
  if(std::get< 0 >(ret) == NO_ERROR)
  {
    this->str_ = std::get< 1 >(ret);
    this->refCount_ = std::get< 2 >(ret);

    std::copy_n(initStr, inLen, this->str_);
  }
  return *this;
}

const char StringWrapper::operator[](u16 pos) const
{
  u16 index = std::min(pos, this->length_);
  return this->str_[index];
}

char StringWrapper::operator[](u16 pos)
{
  u16 index = std::min(pos, this->length_);

  if((*this->refCount_) > 1)
  {
    auto ret = this->strPool_->allocate(this->length_);
    if(std::get< 0 >(ret) == NO_ERROR)
    {
      --(*this->refCount_);
      this->refCount_ = std::get< 2 >(ret);
      this->str_ = std::get< 1 >(ret);
    }
    else
    {
      return 0;
    }
  }
  return this->str_[index];
}

const char* StringWrapper::cStr() { return this->str_; }

// TODO: add string manipulation functions.

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_STRINGS_HPP
