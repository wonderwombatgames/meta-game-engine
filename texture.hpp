/**
  * helper functions to abstrct the backend
  *
  */

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <memory>
#include <string>
#include "graphic_component.hpp"

namespace Engine
{
using namespace std;

template <typename T >
class Texture
{
public:
  Texture(GraphicComponent & component);
  Texture(GraphicComponent & component, const string & filepath, const string & atlas = "");
  Texture() = delete;
  Texture(Texture & other) = delete;
  ~Texture(){};

  bool loadFromFile(const string & filepath, const string & atlas = "");
  bool hasBMP();
  void setPosition(const Vector3 & p);
  const Vector3 & getPosition();
#if 0
  void setScale(const Vector3 & s);
  const Vector3 & getScale();
  void setAnchor(const Vector3 & a);
  const Vector3 & getAnchor();
  void setRotation(const Rotation3 & r);
  const Rotation3 & getRotation();
  void setTinting();
  const Colour & getTinting();
  void setAlpha(const ColourComponent & a);
  const ColourComponent & getAlpha();
  void setBlendMode(const BlendingMode & b);
  const BlendingMode & getBlendMode();
  void setAtlasFrame(const unsigned short & frame);
  const unsigned short & getAtlasFrame();
#endif

protected:
  typedef T Context;

  GraphicComponent * _component;
  unique_ptr< T > _data;
};


} // end namespace Engine

#endif // TEXTURE_HPP
