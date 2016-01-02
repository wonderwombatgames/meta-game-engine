#include <algorithm>
#include "colour_utils.hpp"

namespace Engine
{
using namespace std;

void calcHue(const ColourRGB  & in, ColourComponent & h, const ColourComponent & cMax, const ColourComponent & cMin)
{
  ColourComponent delta = cMax - cMin;

  // calculating Hue
  if( in.r == cMax )
  {
    h = ( in.g - in.b ) / delta;		// between yellow & magenta
  }
  else if( in.g == cMax )
  {
    h = 2.0f + ( in.b - in.r ) / delta;	// between cyan & yellow
  }
  else
  {
    h = 4.0f + ( in.r - in.g ) / delta;	// between magenta & cyan
  }
  h /= 60.0f;				// circle (0.0-1.0)

  if( h < 0 )
  {
    h += 1.0;
  }

}

void Rgb2Hsv (const ColourRGB  & in, ColourHSV  & out)
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
  calcHue(in, out.h, cMax, cMin);

  // calculating value
  out.v = cMax;
}

void Rgb2Hsl (const ColourRGB  & in, ColourHSL  & out)
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
  calcHue(in, out.h, cMax, cMin);

}


void Rgb2Cmyk(const ColourRGB  & in, ColourCMYK & out)
{

}



void Hsl2Rgb (const ColourHSL  & in, ColourRGB  & out)
{

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

void Hsl2Cmyk(const ColourHSL  & in, ColourCMYK & out)
{

}



void Hsv2Rgb (const ColourHSV  & in, ColourRGB  & out)
{

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


void Hsv2Cmyk(const ColourHSV  & in, ColourCMYK & out)
{

}



void Cmyk2Rgb(const ColourCMYK & in, ColourRGB  & out)
{

}


void Cmyk2Hsl(const ColourCMYK & in, ColourHSL  & out)
{

}


void Cmyk2Hsv(const ColourCMYK & in, ColourHSV  & out)
{

}

} // end namespace Engine
