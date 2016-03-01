/**
  * util functions to handle colour data and transofmrations
  *
  */

#ifndef UTILS_COLOUR_HPP
#define UTILS_COLOUR_HPP

#include "utils/types.hpp"

namespace W2E
{

namespace Utils
{

// blender mode enumaration
enum eBlendMode
{
  BLENDMODENONE_,  // no blending: dstRGBA = srcRGBA
  BLENDMODEALPHA_, // alpha blending:  dstRGB = (srcRGB * srcA) + (dstRGB * (1-srcA)) &
                   //                  dstA = srcA + (dstA * (1-srcA))
  BLENDMODEADD_,   // additive blending: dstRGB = (srcRGB * srcA) + dstRGB & dstA = dstA
  BLENDMODEMULT_,  // color modulate: dstRGB = srcRGB * dstRGB & dstA = dstA
};

// colour type enumatration
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

// holda RGB data value
struct ColourRGB
{
  ColourComp r;
  ColourComp g;
  ColourComp b;
};

// holda RGBA data value (RGB +alpha)
struct ColourRGBA
{
  ColourComp r;
  ColourComp g;
  ColourComp b;
  ColourComp a;
};

// holda HSL data value
struct ColourHSL
{
  ColourComp h;
  ColourComp s;
  ColourComp l;
};

// holda HSLA data value (HSL + alpha)
struct ColourHSLA
{
  ColourComp h;
  ColourComp s;
  ColourComp l;
  ColourComp a;
};

// holda HSV data value
struct ColourHSV
{
  ColourComp h;
  ColourComp s;
  ColourComp v;
};

// holda HSVA data value (HSV + alpha)
struct ColourHSVA
{
  ColourComp h;
  ColourComp s;
  ColourComp v;
  ColourComp a;
};

// holda CMYK data value
struct ColourCMYK
{
  ColourComp c;
  ColourComp m;
  ColourComp y;
  ColourComp k;
};

// holda hex data value (html equivalent)
struct ColourHex
{
  char hexVal[8];
};

// general colour data
struct Colour
{
  eColour kind;
  union {
    ColourRGBA rgba;
    ColourHSLA hsla;
    ColourHSVA hsva;
    ColourCMYK cmyk;
    ColourRGB rgb;
    ColourHSL hsl;
    ColourHSV hsv;
    ColourHex hex;
  };
};

// colour default values
namespace Colours
{
GLOBAL const Colour WHITE{RGBA, {{1.0f, 1.0f, 1.0f, 1.0f}}};
GLOBAL const Colour GRAY{RGBA, {{0.5f, 0.5f, 0.5f, 1.0f}}};
GLOBAL const Colour BLACK{RGBA, {{0.0f, 0.0f, 0.0f, 1.0f}}};
GLOBAL const Colour RED{RGBA, {{1.0f, 0.0f, 0.0f, 1.0f}}};
GLOBAL const Colour GREEN{RGBA, {{0.0f, 1.0f, 0.0f, 1.0f}}};
GLOBAL const Colour BLUE{RGBA, {{0.0f, 0.0f, 1.0f, 1.0f}}};
GLOBAL const Colour CYAN{RGBA, {{0.0f, 1.0f, 1.0f, 1.0f}}};
GLOBAL const Colour MAGENTA{RGBA, {{1.0f, 0.0f, 1.0f, 1.0f}}};
GLOBAL const Colour YELLOW{RGBA, {{1.0f, 1.0f, 0.0f, 1.0f}}};
} // end namespace Colours

// conversion from and to different colour formats
void Hex2Rgb(const ColourHex& in, ColourRGB& out);
void Rgb2Hex(const ColourRGB& in, ColourHex& out);

void Rgb2Hsv(const ColourRGB& in, ColourHSV& out);
void Rgb2Hsl(const ColourRGB& in, ColourHSL& out);
void Rgb2Cmyk(const ColourRGB& in, ColourCMYK& out);

void Hsl2Rgb(const ColourHSL& in, ColourRGB& out);
void Hsl2Hsv(const ColourHSL& in, ColourHSV& out);
void Hsl2Cmyk(const ColourHSL& in, ColourCMYK& out);

void Hsv2Rgb(const ColourHSV& in, ColourRGB& out);
void Hsv2Hsl(const ColourHSV& in, ColourHSL& out);
void Hsv2Cmyk(const ColourHSV& in, ColourCMYK& out);

void Cmyk2Rgb(const ColourCMYK& in, ColourRGB& out);
void Cmyk2Hsl(const ColourCMYK& in, ColourHSL& out);
void Cmyk2Hsv(const ColourCMYK& in, ColourHSV& out);

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_COLOUR_HPP
