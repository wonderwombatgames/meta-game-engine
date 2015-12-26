/**
  * simple math utilities and
  * data structs that are POD only
  *
  *
  */

namespace Engine
{


enum eTransformation
{
  TRANSFORMATION_2D,
  TRANSFORMATION_3D,
};

struct Vector3
{
  float x;
  float y;
  // depending on kind this is
  // either the 3rd axis or the z-order
  float z;
};

struct Rotation
{
// rotation
float angleXY;
// these next 2 angles are used only in 3d
float angleYZ;
float angleZX;
};

#define POLYGON_BOUND_MAX_VERTICES 8

enum eBoundary
{
  CIRCLE,
  BOX,
  // POLYGON,
};

struct CircleBoundary
{
  float radius;
};

struct BoxBoundary
{
  Vector3 topLeft;
  Vector3 buttonRight;
};

// struct PolygonBoundary
// {
//   Vector3 * vertices[POLYGON_BOUND_MAX_VERTICES];
// };

struct Boundary
{
  eBoundary kind;
  union
  {
    CircleBoundary  circleBound;
    BoxBoundary     boxBound;
    // PolygonBoundary polyBound;
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
  float r;
  float g;
  float b;
};

struct ColourRGBA
{
  float r;
  float g;
  float b;
  float a;
};

struct ColourHSL
{
  float h;
  float s;
  float l;
};

struct ColourHSLA
{
  float h;
  float s;
  float l;
  float a;
};

struct ColourHSV
{
  float h;
  float s;
  float v;
};

struct ColourHSVA
{
  float h;
  float s;
  float v;
  float a;
};

struct ColourCMYK
{
  float c;
  float m;
  float y;
  float k;
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
