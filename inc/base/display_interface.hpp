/**
  *
  *
  */

#ifndef BASE_DISPLAY_INTERFACE_HPP
#define BASE_DISPLAY_INTERFACE_HPP

#include "utils/colour.hpp"

namespace W2E
{
using namespace Utils;

class DisplayInterface
{
public:
  virtual ~DisplayInterface(){};

  // rendering
  virtual void render() = 0;
  virtual void clear() = 0;
  virtual void clear(const Colour& c) = 0;

  // reseting the viewport
  virtual void setColour(const Colour& c) = 0;
  virtual const Colour& getColour() const = 0;
  virtual void setResolution(Dimension2& res) = 0;
  virtual const Dimension2& getResolution() const = 0;
  virtual void setViewRect(const BoxBoundXYWH& rect) = 0;
  virtual const BoxBoundXYWH& getViewRect() const = 0;
  virtual void setTitle(const String& title) = 0;
  virtual const char* getTitle() const = 0;
  virtual void setFullscreen(bool fs) = 0;
  virtual const bool isFullscreen() const = 0;
};

} // end namespace W2E

#endif // BASE_DISPLAY_INTERFACE_HPP
