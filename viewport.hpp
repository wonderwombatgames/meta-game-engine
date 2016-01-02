/**
  * helper functions to abstrct the backend
  *
  */

#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include <cstdint>
#include <memory>

#include "texture.hpp"

namespace Engine
{
using namespace std;

template < typename T >
class ViewPort
{
public:
  ~ViewPort(){};
  ViewPort(const BoxBoundXYWH & rect, Flags flags = 0);
  ViewPort() = delete;
  ViewPort(ViewPort & other) = delete;

  // rendering
  void render();
  void clear(const Colour * c = nullptr);

  // reseting the viewport
  void setColour(const Colour & c);
  const Colour & getColour() const;
  void setResolution(Dimension3 & res);
  const Dimension3 & getResolution() const;
  void setViewRect(const BoxBoundXYWH & rect);
  const BoxBoundXYWH & getViewRect() const;
  void setTitle(const string & title);
  const char * getTitle() const;
  void setFullscreen(bool fs);
  const bool isFullscreen() const;

protected:
  typedef T Context;

  //data
  unique_ptr< Context > _data;
  BoxBoundXYWH _rect;
  Colour _background;
};


} // end namespace Engine

#endif // VIEWPORT_HPP
