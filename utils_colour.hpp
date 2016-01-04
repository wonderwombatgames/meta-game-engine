/**
  * helper functions to abstrct the backend
  *
  */

#ifndef UTILS_COLOUR_HPP
#define UTILS_COLOUR_HPP

#include "utils_types.hpp"

namespace Engine
{

namespace Utils
{

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

} // end namespace Engine

#endif // UTILS_COLOUR_HPP
