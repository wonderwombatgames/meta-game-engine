#include "utils_containers.hpp"
#include <algorithm>
#include <iostream>

/// FIXME: simplify!!! make it work just as a ring buffer!
/// FIXME: this probably should be a hpp not cpp because of templates
/// TODO: break into 2 containers: stack and de-queue

namespace W2E
{

namespace Utils
{

namespace
{
inline cSize logic2real(cSize first, cSize last, cSize length, cSize total, cSize pos)
{
  cSize index = 0;
  if(pos < length)
  {
    cSize cursor = (pos + first) % total;
    cSize gap = (first > last) ? (first - last) : 0;
    cSize offset = total - (length + gap);
    index = offset + cursor + 1;
  }
  return index;
}
}

const cSize maxStride = 8;

template < typename ElementType, cSize Capacity >
ErrorCode push_back(FixedDeQueue< ElementType, Capacity >& container, const ElementType& el)
{
  if(container.length_ < container.maxLength)
  {
    if(container.lastPos_ >= container.firstPos_)
    {
      ++container.lastPos_;
      container.array_[container.lastPos_] = el;
      ++container.internalLength_;
    }
    else // (container.firstPos_ > container.lastPos_)
    {
      if((container.firstPos_ - container.lastPos_) == 1)
      {
        // reorganize the array_ because the end met the begin circularly
        cSize gap = (container.firstPos_ - container.lastPos_ - 1);
        cSize origFirst = container.internalLength_ - (container.length_ + gap);
        cSize origLast = container.lastPos_;
        cSize destFirst = container.internalLength_;
        if((origLast - origFirst) > (container.maxLength - container.internalLength_))
        {
          if(origFirst > 1)
          {
            destFirst = 0;
          }
          else // (origFirst == 1) || (origFirst == 0)
          {
            container.array_[destFirst] = container.array_[origFirst];
            destFirst = 0;
            ++origFirst;
          }
        }

        for(cSize offset = origFirst; offset <= origLast; ++offset)
        {
          container.array_[destFirst] = container.array_[offset];
          ++destFirst;
        }
        container.lastPos_ = destFirst;
        container.array_[container.lastPos_] = el;
        container.internalLength_ = destFirst + 1;
      }
      else if((container.firstPos_ - container.lastPos_) > 1)
      {
        ++container.lastPos_;
        container.array_[container.lastPos_] = el;
      }
    }
    ++container.length_;
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

template < typename ElementType, cSize Capacity >
ErrorCode push_front(FixedDeQueue< ElementType, Capacity >& container, const ElementType& el)
{
  if(container.length_ < container.maxLength)
  {
    if(container.firstPos_ > 0)
    {
      if(container.lastPos_ >= container.firstPos_)
      {
        --container.firstPos_;
        container.array_[container.firstPos_] = el;
        ++container.internalLength_;
      }
      else
      {
        if((container.firstPos_ - container.lastPos_) == 1)
        {
          // reorganize the array_ because the end met the begin circularly
          cSize gap = (container.firstPos_ - container.lastPos_ - 1);
          cSize origFirst = container.internalLength_ - (container.length_ + gap);
          cSize origLast = container.lastPos_;
          cSize destFirst = container.internalLength_;
          if((origLast - origFirst) > (container.maxLength - container.internalLength_))
          {
            if(origFirst > 1)
            {
              destFirst = 0;
            }
            else // (origFirst == 1) || (origFirst == 0)
            {
              container.array_[destFirst] = container.array_[origFirst];
              destFirst = 0;
              ++origFirst;
            }
          }

          for(cSize offset = origFirst; offset <= origLast; ++offset)
          {
            container.array_[destFirst] = container.array_[offset];
            container.lastPos_ = destFirst;
            ++destFirst;
          }
          --container.firstPos_;
          container.array_[--container.firstPos_] = el;
          container.internalLength_ = destFirst;
        }
        else if((container.firstPos_ - container.lastPos_) > 1)
        {
          --container.firstPos_;
          container.array_[container.firstPos_] = el;
        }
      }
    }
    else // (container.firstPos_ == 0)
    {
      // have to move first after last
      cSize delta = container.maxLength - container.length_;
      cSize stride = (delta > maxStride) ? maxStride + 1 : (delta >> 1) + 1;
      container.firstPos_ = container.lastPos_ + stride;
      container.array_[--container.firstPos_] = el;
      container.internalLength_ = container.internalLength_ + stride;
    }
    ++container.length_;
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

template < typename ElementType, cSize Capacity >
ElementType pop_back(FixedDeQueue< ElementType, Capacity >& container, ElementType fallback)
{
  if(container.length_ > 0)
  {
    if(container.lastPos_ > container.firstPos_)
    {
      --container.internalLength_;
    }
    --container.length_;
    --container.lastPos_;
    return container.array_[container.lastPos_ + 1];
  }
  return fallback;
}

template < typename ElementType, cSize Capacity >
ElementType pop_front(FixedDeQueue< ElementType, Capacity >& container, ElementType fallback)
{
  if(container.length_ > 0)
  {
    if(container.lastPos_ > container.firstPos_)
    {
      --container.internalLength_;
    }
    --container.length_;
    ++container.firstPos_;
    return container.array_[container.firstPos_ - 1];
  }
  return fallback;
}

template < typename ElementType, cSize Capacity >
ElementType* front(FixedDeQueue< ElementType, Capacity >& container)
{
  return &(container.array_[container.firstPos_]);
}

template < typename ElementType, cSize Capacity >
ElementType* back(FixedDeQueue< ElementType, Capacity >& container)
{
  return &(container.array_[container.lastPos_]);
}

template < typename ElementType, cSize Capacity >
ElementType* at(FixedDeQueue< ElementType, Capacity >& container, cSize pos)
{
  if(pos < container.length_)
  {
    cSize index = logic2real(
        container.firstPos_, container.lastPos_, container.length_, container.internalLength_, pos);

    if(index < container.internalLength_)
    {
      return &(container.array_[index]);
    }
  }
  return nullptr;
}

template < typename ElementType, cSize Capacity >
ErrorCode del(FixedDeQueue< ElementType, Capacity >& container, cSize pos)
{
  if(pos < container.length_)
  {
    if(pos < (container.length_ / 2))
    {
      for(cSize ndx = pos; ndx > 0; --ndx)
      {
        cSize to = logic2real(container.firstPos_,
                              container.lastPos_,
                              container.length_,
                              container.internalLength_,
                              ndx);
        cSize from = logic2real(container.firstPos_,
                                container.lastPos_,
                                container.length_,
                                container.internalLength_,
                                (ndx - 1));
        std::cout << "del pos -> to / from " << ndx << " -> " << to << " / " << from << std::endl;
        container.array_[to] = container.array_[from];
      }
      --container.internalLength_;
      --container.length_;
      ++container.firstPos_;
    }
    else
    {
      for(cSize ndx = pos; ndx < container.length_; ++ndx)
      {
        cSize to = logic2real(container.firstPos_,
                              container.lastPos_,
                              container.length_,
                              container.internalLength_,
                              ndx);
        cSize from = logic2real(container.firstPos_,
                                container.lastPos_,
                                container.length_,
                                container.internalLength_,
                                (ndx + 1));
        container.array_[to] = container.array_[from];
      }
      --container.internalLength_;
      --container.length_;
      --container.lastPos_;
    }
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

template < typename ElementType, cSize Capacity >
cSize length(FixedDeQueue< ElementType, Capacity >& container)
{
  return container.length_;
}

} // end namespace Utils

} // end namespace W2E
