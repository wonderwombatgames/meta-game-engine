/**
  * helper functions to abstrct the backend
  *
  */

#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include <cstdint>
#include <memory>

#include "data_utils.hpp"
#include "texture.hpp"

namespace Engine
{
using namespace std;

template < typename T >
class ViewPort
{
public:
  ~ViewPort(){};
  ViewPort(const BoxBoundary & rect, Flags flags = 0);
  ViewPort() = delete;
  ViewPort(ViewPort & other) = delete;

  // rendering
  void render();
  void clear();

  // reseting the viewport
  void setColour(const Colour & c);
  void setFullscreen(bool fs);
  bool isFullscreen() const;
  void setResolution(Vector3 & res);
  const Vector3 & getResolution() const;
  void setViewBox(const BoxBoundary & rect);
  const BoxBoundary & getViewBox() const;

protected:
  typedef T Context;

  //data
  unique_ptr< Context > _data;
  BoxBoundary _rect;
  Vector3 _res;
  Colour _background;
};


} // end namespace Engine

#endif // VIEWPORT_HPP
