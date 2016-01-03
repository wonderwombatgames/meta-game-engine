#include <algorithm>
#include "colour_utils.hpp"

namespace Engine
{
using namespace std;

inline uint8_t hexChar2Val(const char * ch)
{
  uint8_t val = 0;
  uint8_t in = *ch;
  if ((in >= 48) && (in <= 57)) // 0-9
  {
    val = in - 48;
  }
  else if ((in >= 65) && (in <= 70)) // A-F
  {
    val = in - 55;
  }
  else if ((in >= 97) && (in <= 102)) // a-f
  {
    val = in - 87;
  }
  return val;
}


void Hex2Rgb (const ColourHex  & in, ColourRGB  & out)
{
  uint32_t rgb = 0;
  for (uint8_t i = 0; i < 8; i++)
  {
    rgb += hexChar2Val(&in.hexVal[i]);
    rgb <<= 1;
  }
  out.b = static_cast<ColourComponent>(0xff & rgb);
  out.g = static_cast<ColourComponent>(0xff & (rgb >> 8));
  out.r = static_cast<ColourComponent>(0xff & (rgb >> 16));
}


void Rgb2Hex (const ColourRGB  & in, ColourHex  & out)
{
  const char hex[16] = {
      '0','1','2','3',
      '4','5','6','7',
      '8','9','a','b',
      'c','d','e','f'};

  uint8_t r = (static_cast<uint8_t>(in.r * 255) % 256);
  uint8_t g = (static_cast<uint8_t>(in.g * 255) % 256);
  uint8_t b = (static_cast<uint8_t>(in.b * 255) % 256);

  uint8_t rLo = r & 0x0f;
  uint8_t rHi = (r >> 4) & 0x0f;
  uint8_t gLo = g & 0x0f;
  uint8_t gHi = (g >> 4) & 0x0f;
  uint8_t bLo = b & 0x0f;
  uint8_t bHi = (b >> 4) & 0x0f;

  out.hexVal[7] = '\0';
  out.hexVal[6] = hex[rHi];
  out.hexVal[5] = hex[rLo];
  out.hexVal[4] = hex[gHi];
  out.hexVal[3] = hex[gLo];
  out.hexVal[2] = hex[bHi];
  out.hexVal[1] = hex[bLo];
  out.hexVal[0] = '#';

}


ColourComponent calcHueFromRgb(
    const ColourRGB  & in,
    const ColourComponent & cMax,
    const ColourComponent & cMin)
{
  ColourComponent delta = cMax - cMin;
  ColourComponent h = 0.0f;

  // calculating Hue
  if( in.r == cMax )
  {
    // between yellow & magenta
    h = ( in.g - in.b ) / delta;
  }
  else if( in.g == cMax )
  {
    // between cyan & yellow
    h = 2.0f + ( in.b - in.r ) / delta;
  }
  else
  {
    // between magenta & cyan
    h = 4.0f + ( in.r - in.g ) / delta;
  }

  // normalize h value
  h = fmod(h + 6.0f, 6.0f); // 0.0 <= h < 6.0
  h /= 6.0f; // circle (0.0-1.0)

  return h;
}


void Rgb2Hsv(const ColourRGB  & in, ColourHSV  & out)
{
  ColourComponent cMax = std::max(in.r, std::max(in.g, in.b));
  ColourComponent cMin = std::min(in.r, std::min(in.g, in.b));
  ColourComponent delta = cMax - cMin;

  //calculating saturation
  if( cMax > 0 )
  {
    out.s = delta / cMax; // s
  }
  else
  {
    // r = g = b = 0		// s = 0, v is undefined
    out.s = 0;
    out.h = 0;
    out.v = 0;
    return;
  }

  // calculating hue
  out.h = calcHueFromRgb(in, cMax, cMin);

  // calculating value
  out.v = cMax;
}


void Rgb2Hsl(const ColourRGB  & in, ColourHSL  & out)
{
  ColourComponent cMax = std::max(in.r, std::max(in.g, in.b));
  ColourComponent cMin = std::min(in.r, std::min(in.g, in.b));
  ColourComponent delta = cMax - cMin;

  // calculating lightness
  out.l = 0.5 * (cMax + cMin);

  //calculating saturation
  if( delta > 0 )
  {
    out.s = delta / (1 - abs((2*out.l) - 1)); // s
  }
  else
  {
    // r = g = b = 0		// s = 0, v is undefined
    out.s = 0;
    out.h = 0;
    out.l = 0;
    return;
  }

  // calculating Hue
  out.h = calcHueFromRgb(in, cMax, cMin);
}


void Rgb2Cmyk(const ColourRGB  & in, ColourCMYK & out)
{
  out.k = 1 - max(in.r, max(in.g, in.b));
  out.c = (1 - in.r - out.k) / (1 - out.k);
  out.m = (1 - in.g - out.k) / (1 - out.k);
  out.y = (1 - in.b - out.k) / (1 - out.k);
}


inline void calcRgbFromHue(
  const ColourComponent & h,
  const ColourComponent & c,
  const ColourComponent & x,
  const ColourComponent & m,
  ColourRGB & out)
{
  out.r = 0.0f;
  out.g = 0.0f;
  out.b = 0.0f;

  if ((h >= 0) && (h < 1))
  {
    out.r = m + c;
    out.g = m + x;
    out.b = m + 0.0f;
  }
  else if ((h >= 1) && (h < 2))
  {
    out.r = m + x;
    out.g = m + c;
    out.b = m + 0.0f;
  }
  else if ((h >= 2) && (h < 3))
  {
    out.r = m + 0.0f;
    out.g = m + c;
    out.b = m + x;
  }
  else if ((h >= 3) && (h < 4))
  {
    out.r = m + 0.0f;
    out.g = m + x;
    out.b = m + c;
  }
  else if ((h >= 4) && (h < 5))
  {
    out.r = m + x;
    out.g = m + 0.0f;
    out.b = m + c;
  }
  else if ((h >= 5) && (h < 6))
  {
    out.r = m + c;
    out.g = m + 0.0f;
    out.b = m + x;
  }
}


void Hsl2Rgb (const ColourHSL  & in, ColourRGB  & out)
{
  ColourComponent h = fmod(6.0f + in.h * 6.0f, 6.0f);
  ColourComponent c = in.s * (1 - abs(2 * in.l - 1));
  ColourComponent x = c * (1 - abs( fmod(h, 2.0f) -1));
  ColourComponent m = in.l - (c / 2.0f);

  calcRgbFromHue(h, c, x, m, out);
}


void Hsv2Rgb (const ColourHSV  & in, ColourRGB  & out)
{
  ColourComponent h = fmod(6.0f + in.h * 6.0f, 6.0f);
  ColourComponent c = in.v * in.s;
  ColourComponent x = c * (1 - abs( fmod(h, 2.0f) -1));
  ColourComponent m = in.v - c;

  calcRgbFromHue(h, c, x, m, out);
}


void Cmyk2Rgb(const ColourCMYK & in, ColourRGB  & out)
{
  out.r = (1-in.c)*(1-in.k);
  out.g = (1-in.m)*(1-in.k);
  out.b = (1-in.y)*(1-in.k);
}


void Hsl2Hsv (const ColourHSL  & in, ColourHSV  & out)
{
  //Saturation is very different between the two color spaces
  //If (2-sat)*val < 1 set it to sat*val/((2-sat)*val)
  //Otherwise sat*val/(2-(2-sat)*val)

  // repeat the hue without change
  out.h = in.h;

  ColourComponent l2 = 2 * in.l;
  ColourComponent s2 = 0.0f;

  if(l2 <= 1 )
  {
    s2 = in.s * l2;
  }
  else
  {
    s2 = in.s * (2 - l2);
  }

  out.v = (l2 + s2) / 2;
  out.s = (2 * s2) / (l2 + s2);

}


void Hsv2Hsl (const ColourHSV  & in, ColourHSL  & out)
{
  ColourComponent cMax = in.v;
  ColourComponent cMin = in.v - in.s*in.v;
  ColourComponent delta = cMax - cMin;

  // repeat the hue without change
  out.h = in.h;

  // calculating lightness
  out.l = 0.5 * (cMax + cMin);

  //calculating saturation
  if( delta > 0 )
  {
    out.s = delta / (1 - abs((2*out.l) - 1)); // s
  }
  else
  {
    // r = g = b = 0		// s = 0, v is undefined
    out.s = 0;
    out.h = 0;
    out.l = 0;
    return;
  }

}


void Hsl2Cmyk(const ColourHSL  & in, ColourCMYK & out)
{
  ColourRGB tmp;
  Hsl2Rgb(in, tmp);
  Rgb2Cmyk(tmp, out);
}


void Hsv2Cmyk(const ColourHSV  & in, ColourCMYK & out)
{
  ColourRGB tmp;
  Hsv2Rgb(in, tmp);
  Rgb2Cmyk(tmp, out);
}


void Cmyk2Hsl(const ColourCMYK & in, ColourHSL  & out)
{
  ColourRGB tmp;
  Cmyk2Rgb(in, tmp);
  Rgb2Hsl(tmp, out);
}


void Cmyk2Hsv(const ColourCMYK & in, ColourHSV  & out)
{
  ColourRGB tmp;
  Cmyk2Rgb(in, tmp);
  Rgb2Hsv(tmp, out);
}

} // end namespace Engine
