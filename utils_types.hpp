/**
  * simple math utilities and
  * data structs that are POD only
  *
  *
  */

#ifndef UTILS_TYPES_HPP
#define UTILS_TYPES_HPP

#include <cstdint>
#include <memory>
#include <set>
#include <unordered_map>
#include <deque>
#include <string>

namespace W2E
{

namespace Utils
{

// based on Ginger Bill DUNJUN
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

// containers
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

// game specific types

using FrameCount = u64;
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

// creates a new random ID < MAX_IDS
EntityID rndId();
EntityID seqId();

union Vector2
{
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

union Vector3
{
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

union Vector4
{
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
GLOBAL const Vector3 TOP_LEFT{{0.0f, 0.0f, 0.0f}};
GLOBAL const Vector3 TOP_RIGHT{{1.0f, 0.0f, 0.0f}};
GLOBAL const Vector3 BOT_LEFT{{0.0f, 1.0f, 0.0f}};
GLOBAL const Vector3 BOT_RIGHT{{1.0f, 1.0f, 0.0f}};
GLOBAL const Vector3 ZERO{{0.0f, 0.0f, 0.0f}};
GLOBAL const Vector3 HALF{{0.5f, 0.5f, 0.0f}};
GLOBAL const Vector3 ONE{{1.0f, 1.0f, 0.0f}};

enum eBound
{
  CIRCLE,
  BOX_TLBR,
  BOX_XYWH,
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
  eBound kind = BOX_XYWH;
  union
  {
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

#endif // UTILS_TYPES_HPP
