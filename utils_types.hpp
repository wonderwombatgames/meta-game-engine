/**
  * simple math utilities and
  * data structs that are POD only
  *
  *
  */

#ifndef UTILSTYPES_HPP_
#define UTILSTYPES_HPP_

#include <cassert>
#include <cstdint>
#include <deque>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>

namespace W2E
{

namespace Utils
{

// safe ptr deref with asssert
template < typename T >
inline T& Deref(T* ptr)
{
  assert(ptr != nullptr);
  return *ptr;
}

// based idea from Ginger Bill DUNJUN: github.com/gingerBill/Dunjun/
#define GLOBAL static           // global variables
#define CLASS_METHOD static     // class methods work without instantiation
#define INTERNAL static         // internal linkage
#define LOCAL_PERSISTENT static // local persisting variables

// basic types
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using f32 = float;
using f64 = double;

// game specific size type
using cSize = i64;

// this adds modulus semantics to integers
template < typename IntegerType, cSize modulusVal >
class Modulus
{
  void mod()
  {
    if(this->value_ <= -(this->modulusVal_))
    {
      this->value_ %= this->modulusVal_;
    }
    this->value_ += this->modulusVal_;
    this->value_ %= this->modulusVal_;
  }
  Modulus() = delete;
  const GLOBAL cSize modulusVal_;
  IntegerType value_;

public:
  Modulus(IntegerType val)
      : value_{val}
  {
    this->mod();
  }
  explicit Modulus(Modulus& other)
      : value_{other.value_}
  {
    this->mod();
  }

  Modulus& operator=(IntegerType val)
  {
    this->value_ = val;
    this->mod();
    return *this;
  }
  Modulus& operator=(Modulus& other)
  {
    this->value_ = other.value_;
    this->mod();
    return *this;
  }

  Modulus operator+(IntegerType val)
  {
    this->value_ += val;
    this->mod();
    return *this;
  }
  Modulus operator+(Modulus& other)
  {
    this->value_ += other.value_;
    this->mod();
    return *this;
  }

  Modulus operator-(IntegerType val)
  {
    this->value_ -= val;
    this->mod();
    return *this;
  }
  Modulus operator-(Modulus& other)
  {
    this->value_ -= other.value_;
    this->mod();
    return *this;
  }

  Modulus operator*(IntegerType val)
  {
    this->value_ *= val;
    this->mod();
    return *this;
  }
  Modulus operator*(Modulus& other)
  {
    this->value_ *= other.value_;
    this->mod();
    return *this;
  }

  Modulus operator/(IntegerType val)
  {
    assert(val > 0);
    this->value_ = static_cast< IntegerType >(this->value_ / val);
    this->mod();
    return *this;
  }
  Modulus operator/(Modulus& other)
  {
    assert(other.value_ > 0);
    this->value_ = other.value_;
    this->mod();
    return *this;
  }

  Modulus& operator+=(IntegerType val)
  {
    this->value_ += val;
    this->mod();
    return *this;
  }

  Modulus& operator+=(Modulus& other)
  {
    this->value_ += other.value_;
    this->mod();
    return *this;
  }

  Modulus& operator-=(IntegerType val)
  {
    this->value_ -= val;
    this->mod();
    return *this;
  }
  Modulus& operator-=(Modulus& other)
  {
    this->value_ -= other.value_;
    this->mod();
    return *this;
  }

  Modulus& operator++()
  {
    this->value_ += 1;
    this->mod();
    return *this;
  }

  Modulus& operator--()
  {
    this->value_ -= 1;
    this->mod();
    return *this;
  }

  bool operator==(IntegerType val) { return this->value_ == val; }

  bool operator==(Modulus& other) { return this->value_ == other.value_; }

  bool operator!=(IntegerType val) { return this->value_ != val; }

  bool operator!=(Modulus& other) { return this->value_ != other.value_; }

  bool operator>=(IntegerType val) { return this->value_ >= val; }

  bool operator>=(Modulus& other) { return this->value_ >= other.value_; }

  bool operator<=(IntegerType val) { return this->value_ <= val; }

  bool operator<=(Modulus& other) { return this->value_ <= other.value_; }

  bool operator>(IntegerType val) { return this->value_ > val; }

  bool operator>(Modulus& other) { return this->value_ > other.value_; }

  bool operator<(IntegerType val) { return this->value_ < val; }

  bool operator<(Modulus& other) { return this->value_ < other.value_; }

  const IntegerType& toInt() const { return this->value_; }
};

template < typename IntegerType, cSize modulusVal >
const cSize Modulus< IntegerType, modulusVal >::modulusVal_ = modulusVal;

template < cSize modulusVal >
using Modulus8 = Modulus< i8, modulusVal >;

template < cSize modulusVal >
using Modulus16 = Modulus< i16, modulusVal >;

template < cSize modulusVal >
using Modulus32 = Modulus< i32, modulusVal >;

template < cSize modulusVal >
using Modulus64 = Modulus< i64, modulusVal >;

// wrappers around STD and STL.
// TODO: STL containers and string will be replaced.
template < typename T >
using SharedPtr = std::shared_ptr< T >;

template < typename T >
using UniquePtr = std::unique_ptr< T >;

template < typename T >
using Set = std::set< T >;

template < typename T >
using List = std::deque< T >;

template < typename K, typename T >
using HashMap = std::unordered_map< K, T >;

template < typename K, typename T >
using HashMultiMap = std::unordered_multimap< K, T >;

using String = std::string;

using Flags = u32;
using BlendingMode = u16;
using ColourComp = f32;
using SpaceDim = f32;
using TimeDim = f32;

using EntityID = u32;
using TypeID = EntityID;
using ResourceID = EntityID;

using CommandType = u16;
using CommandOp = u16;

enum
{
  InvalidID = -1
};
enum ErrorCode
{
  UNKNOWN_ERROR = 0,
  NO_ERROR = 1,
};

// creates a new random ID < MAXIDS_
EntityID rndId();
EntityID seqId();

union Vector2 {
  struct
  {
    SpaceDim x;
    SpaceDim y;
  };
  struct
  {
    SpaceDim u;
    SpaceDim v;
  };
  struct
  {
    SpaceDim s;
    SpaceDim t;
  };
  struct
  {
    SpaceDim width;
    SpaceDim height;
  };
  struct
  {
    SpaceDim roll;
    SpaceDim pitch;
  };
};

union Vector3 {
  struct
  {
    SpaceDim x;
    SpaceDim y;
    SpaceDim z;
  };
  struct
  {
    SpaceDim width;
    SpaceDim height;
    SpaceDim depth;
  };
  struct
  {
    SpaceDim pitch;
    SpaceDim roll;
    SpaceDim yaw;
  };
};

union Vector4 {
  struct
  {
    SpaceDim x;
    SpaceDim y;
    SpaceDim z;
    SpaceDim w;
  };
  struct
  {
    SpaceDim pitch;
    SpaceDim roll;
    SpaceDim yaw;
    SpaceDim mag;
  };
};

using Dimension2 = Vector2;
using Dimension3 = Vector3;

using Rotation2 = Vector2;
using Rotation3 = Vector3;

GLOBAL const Vector3 CENTER{{0.5f, 0.5f, 0.0f}};
GLOBAL const Vector3 TOPLEFT_{{0.0f, 0.0f, 0.0f}};
GLOBAL const Vector3 TOPRIGHT_{{1.0f, 0.0f, 0.0f}};
GLOBAL const Vector3 BOTLEFT_{{0.0f, 1.0f, 0.0f}};
GLOBAL const Vector3 BOTRIGHT_{{1.0f, 1.0f, 0.0f}};
GLOBAL const Vector3 ZERO{{0.0f, 0.0f, 0.0f}};
GLOBAL const Vector3 HALF{{0.5f, 0.5f, 0.0f}};
GLOBAL const Vector3 ONE{{1.0f, 1.0f, 0.0f}};

enum eBound
{
  CIRCLE,
  BOXTLBR_,
  BOXXYWH_,
};

struct CircleBound
{
  SpaceDim radius; // = 0.0f;
};

struct BoxBoundTLBR
{
  Vector2 topLeft;
  Vector2 bottonRight;
};

struct BoxBoundXYWH
{
  Vector2 topLeft;
  Dimension2 size;
};

using BoxBoundAABB = BoxBoundTLBR;

struct Bound
{
  eBound kind = BOXXYWH_;
  union {
    CircleBound circleBound;
    BoxBoundTLBR boxBoundTLBR;
    BoxBoundXYWH boxBoundXYWH;
  };
};

enum eAudioState
{
  PLAYING,
  PAUSED,
  STOPED,
};

} // end namespace Utils

} // end namespace W2E

#endif // UTILSTYPES_HPP_
