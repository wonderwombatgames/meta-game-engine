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

namespace Engine
{

namespace Utils
{

// based on Ginger Bill DUNJUN
#define GLOBAL        static // global variables
#define INTERNAL      static // internal linkage
#define LOCAL_PERSIST static // local persisting variables

// basic types
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using f32 = float;
using f64 = double;


// containers
template<typename el>
using SharedPtr = std::shared_ptr<el>;

template<typename el>
using UniquePtr = std::unique_ptr<el>;

template<typename el>
using Set = std::set<el>;

template<typename el>
using List = std::deque<el>;

template<typename k, typename el>
using HashMap = std::unordered_map<k,el>;

template<typename k, typename el>
using HashMultiMap = std::unordered_multimap<k,el>;

using String = std::string;


// game specific types
enum { InvalidID = -1 };

using FrameCount = u64;
using Flags = u32;
using BlendingMode = u16;
using ErrorCode = u8;
using ColourComp = f32;
using SpaceDim = f32;
using TimeDim = f32;

using EntityID = u32 ;
using TypeID = EntityID;
using ResourceID = EntityID;

// creates a new random ID < MAX_IDS
EntityID rndId();
EntityID seqId();

enum eSpace
{
  SPACE_2D,
  SPACE_3D,
};


union Vector2
{
  struct {
    SpaceDim x;
    SpaceDim y;
  };
  struct {
    SpaceDim u;
    SpaceDim v;
  };
  struct {
    SpaceDim s;
    SpaceDim t;
  };
  struct {
    SpaceDim width;
    SpaceDim height;
  };
  struct {
    SpaceDim roll;
    SpaceDim pitch;
  };
};


union Vector3
{
  struct{
    SpaceDim x;
    SpaceDim y;
    SpaceDim z;
  };
  struct{
    SpaceDim width;
    SpaceDim height;
    SpaceDim depth;
  };
  struct{
    SpaceDim pitch;
    SpaceDim roll;
    SpaceDim yaw;
  };
};


union Vector4
{
  struct{
    SpaceDim x;
    SpaceDim y;
    SpaceDim z;
    SpaceDim w;
  };
  struct{
    SpaceDim pitch;
    SpaceDim roll;
    SpaceDim yaw;
    SpaceDim mag;
  };
};

using Dimension2 = Vector2 ;
using Dimension3 = Vector3;

using Rotation2 = Vector2;
using Rotation3 = Vector3;


enum eBound
{
  CIRCLE,
  BOX_TLBR,
  BOX_XYWH,
};

struct CircleBound
{
  SpaceDim radius;// = 0.0f;
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
    CircleBound   circleBound;
    BoxBoundTLBR  boxBoundTLBR;
    BoxBoundXYWH  boxBoundXYWH;
  };
};

enum eAudioState
{
  PLAYING,
  PAUSED,
  STOPED,
};

} // end namespace Utils

} // end namespace Engine

#endif // UTILS_TYPES_HPP
