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
  ViewPort(BoxBoundary & rect, Flags flags = 0);
  ViewPort() = delete;
  ViewPort(ViewPort & other) = delete;

  // rendering
  void paint(Texture< T > & tex);
  void render();
  void clear();

  void setColour(const Colour & c);
  void setFullscreen(bool fs);
  bool isFullscreen() const;
  void setResolution(Vector3 & res);
//  const Vector3 & getResolution() const;
//  const BoxBoundary & getView() const;

protected:
  typedef T Context;

  //data
  unique_ptr< T > _data;
};


} // end namespace Engine

#endif // VIEWPORT_HPP
