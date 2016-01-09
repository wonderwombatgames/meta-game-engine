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

enum { InvalidID = -1 };

typedef uint32_t Flags;
typedef uint16_t BlendingMode;
typedef uint8_t ErrorCode;
typedef float ColourComponent;
typedef float SpatialDimention;

typedef long int EntityID;
typedef EntityID TypeID;
typedef EntityID AssetID;


// EntityID newId()
// {
//   const EntityID maxEntities = EntitiesManager::MAX_ENTITIES_AMOUNT;
//   // use current time as seed for random generator
//   srand(time(0));
//   EntityID randomId = 1 + ((rand() * (maxEntities + 1)) % maxEntities);
//   int attempts = 0;
//   // check that the id was not used yet
//   while (this->_entities.count(randomId)>0)
//   {
//     randomId = (rand() * (maxEntities + 1)) % maxEntities;
//     ++attempts;
//     // this ensures we do not allocate more entities than the MAX
//     assert( attempts < maxEntities);
//   }
//   return randomId;
// }

enum eSpace
{
  SPACE_2D,
  SPACE_3D,
};


struct Vector3
{
  SpatialDimention x;
  SpatialDimention y;
  // depending on kind of space this might be
  // either the 3rd axis or the z-order.
  // in case it is z-order it is casted to integer.
  SpatialDimention z;
};


struct Rotation3
{
  // rotation
  SpatialDimention angleXY;
  // these next 2 angles are used only in 3d
  SpatialDimention angleYZ;
  SpatialDimention angleZX;
};


struct Dimension3
{
  SpatialDimention w; // width
  SpatialDimention h; // height
  SpatialDimention d; // depth
};


enum eBound
{
  CIRCLE,
  BOX_TLBR,
  BOX_XYWH,
  // MULTICIRCLE,
  // MULTIBOX,
  // POLYGON,
  // HASH,
};

struct CircleBound
{
  SpatialDimention radius;
};

struct BoxBoundTLBR
{
  Vector3 topLeft;
  Vector3 bottonRight;
};

typedef BoxBoundTLBR BoxBoundAABB;

struct BoxBoundXYWH
{
  Vector3 topLeft;
  Dimension3 size;
};


// #define POLYGON_BOUND_MAX_VERTICES 16
// struct PolygonBound
// {
//   unsigned char numOfVertice;
//   Vector3 * vertices[POLYGON_BOUND_MAX_VERTICES];
// };
;



struct Bound
{
  eBound kind;
  union
  {
    CircleBound  circleBound;
    BoxBoundTLBR     boxBoundTLBR;
    BoxBoundXYWH     boxBoundXYWH;
    // MultiBoxBound multiBoxBound;
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
  ColourComponent r;
  ColourComponent g;
  ColourComponent b;
};

struct ColourRGBA
{
  ColourComponent r;
  ColourComponent g;
  ColourComponent b;
  ColourComponent a;
};

struct ColourHSL
{
  ColourComponent h;
  ColourComponent s;
  ColourComponent l;
};

struct ColourHSLA
{
  ColourComponent h;
  ColourComponent s;
  ColourComponent l;
  ColourComponent a;
};

struct ColourHSV
{
  ColourComponent h;
  ColourComponent s;
  ColourComponent v;
};

struct ColourHSVA
{
  ColourComponent h;
  ColourComponent s;
  ColourComponent v;
  ColourComponent a;
};

struct ColourCMYK
{
  ColourComponent c;
  ColourComponent m;
  ColourComponent y;
  ColourComponent k;
};

struct ColourHex
{
  char hexVal[8];
};

struct Colour
{
  eColour kind;
  union
  {
    ColourRGB rgb;
    ColourRGBA rgba;
    ColourHSL hsl;
    ColourHSLA hsla;
    ColourHSV hsv;
    ColourHSVA hsva;
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
