/**
  * simple math utilities and
  * data structs that are POD only
  *
  *
  */

#ifndef UTILS_TYPES_HPP
#define UTILS_TYPES_HPP

#include <cstdint>


namespace Engine
{

namespace Utils
{

// basic types
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef float    f32;
typedef double   f64;


// game specific types
enum { InvalidID = -1 };

typedef u64 FrameCount;
typedef u32 Flags;
typedef u16 BlendingMode;
typedef u8  ErrorCode;
typedef f32 ColourComp;
typedef f32 SpaceDim;
typedef f32 TimeDim;

typedef u32 EntityID;
typedef EntityID TypeID;
typedef EntityID AssetID;

// creates a new random ID < MAX_IDS
EntityID newId();

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
    SpaceDim width;
    SpaceDim height;
    SpaceDim depth;
    TimeDim  time;
  };
  struct{
    SpaceDim pitch;
    SpaceDim roll;
    SpaceDim yaw;
    SpaceDim mag;
  };
};

typedef Vector2 Dimension2;
typedef Vector3 Dimension3;

typedef Vector2 Rotation2;
typedef Vector3 Rotation3;


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

typedef BoxBoundTLBR BoxBoundAABB;

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

enum eColour
{
  RGB,
  RGBA,
  HSL,
  HSLA,
  HSV,
  HSVA,
  CMYK,
  HEX,
};

struct ColourRGB
{
  ColourComp r;
  ColourComp g;
  ColourComp b;
};

struct ColourRGBA
{
  ColourComp r;
  ColourComp g;
  ColourComp b;
  ColourComp a;
};

struct ColourHSL
{
  ColourComp h;
  ColourComp s;
  ColourComp l;
};

struct ColourHSLA
{
  ColourComp h;
  ColourComp s;
  ColourComp l;
  ColourComp a;
};

struct ColourHSV
{
  ColourComp h;
  ColourComp s;
  ColourComp v;
};

struct ColourHSVA
{
  ColourComp h;
  ColourComp s;
  ColourComp v;
  ColourComp a;
};

struct ColourCMYK
{
  ColourComp c;
  ColourComp m;
  ColourComp y;
  ColourComp k;
};

struct ColourHex
{
  char hexVal[8];
};

struct Colour
{
  eColour kind = RGBA;
  union
  {
    ColourRGBA rgba = {  .r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 0.0f };
    ColourRGB rgb;
    ColourHSLA hsla;
    ColourHSL hsl;
    ColourHSVA hsva;
    ColourHSV hsv;
    ColourCMYK cmyk;
    ColourHex hex;
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
