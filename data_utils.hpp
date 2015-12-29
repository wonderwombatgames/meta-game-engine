/**
  * simple math utilities and
  * data structs that are POD only
  *
  *
  */

#ifndef DATA_UTILS_HPP
#define DATA_UTILS_HPP

namespace Engine
{

typedef float ColourComponent;
typedef float SpatialDimention;

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


enum eBoundary
{
  CIRCLE,
  BOX,
  POLYGON,
  MULTIBOX,
};

struct CircleBoundary
{
  SpatialDimention radius;
};

struct BoxBoundary
{
  Vector3 topLeft;
  Vector3 buttonRight;
};

// #define POLYGON_BOUND_MAX_VERTICES 16
// struct PolygonBoundary
// {
//   unsigned char numOfVertice;
//   Vector3 * vertices[POLYGON_BOUND_MAX_VERTICES];
// };

// #define MULTIBOX_BOUND_MAX_BOXES 8
// struct MultiBoxBoundary
// {
//   unsigned char numOfBoxes;
//   Vector3 * topLefts[MULTIBOX_BOUND_MAX_BOXES];
//   Vector3 * bottomRigths[MULTIBOX_BOUND_MAX_BOXES];
// };


struct Boundary
{
  eBoundary kind;
  union
  {
    CircleBoundary  circleBound;
    BoxBoundary     boxBound;
    // PolygonBoundary polyBound;
    // MultiBoxBoundary multiBoxBound;
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

enum eSoundState
{
  PLAYING,
  PAUSED,
  STOPED,
};

} // end namespace Engine

#endif // DATA_UTILS_HPP
