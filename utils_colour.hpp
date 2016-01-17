/**
  * helper functions to abstrct the backend
  *
  */

#ifndef UTILS_COLOUR_HPP
#define UTILS_COLOUR_HPP

#include "utils_types.hpp"

namespace W2E
{

namespace Utils
{

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

void Hex2Rgb (const ColourHex  & in, ColourRGB  & out);
void Rgb2Hex (const ColourRGB  & in, ColourHex  & out);

void Rgb2Hsv (const ColourRGB  & in, ColourHSV  & out);
void Rgb2Hsl (const ColourRGB  & in, ColourHSL  & out);
void Rgb2Cmyk(const ColourRGB  & in, ColourCMYK & out);

void Hsl2Rgb (const ColourHSL  & in, ColourRGB  & out);
void Hsl2Hsv (const ColourHSL  & in, ColourHSV  & out);
void Hsl2Cmyk(const ColourHSL  & in, ColourCMYK & out);

void Hsv2Rgb (const ColourHSV  & in, ColourRGB  & out);
void Hsv2Hsl (const ColourHSV  & in, ColourHSL  & out);
void Hsv2Cmyk(const ColourHSV  & in, ColourCMYK & out);

void Cmyk2Rgb(const ColourCMYK & in, ColourRGB  & out);
void Cmyk2Hsl(const ColourCMYK & in, ColourHSL  & out);
void Cmyk2Hsv(const ColourCMYK & in, ColourHSV  & out);

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_COLOUR_HPP
