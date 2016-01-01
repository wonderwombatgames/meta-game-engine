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

class IGraphics
{
public:
  IGraphics(){}
  virtual ~IGraphics(){}
  virtual void paint() = 0;
};

template <typename T >
class Texture : public IGraphics
{
public:
  Texture(GraphicComponent & component);
  Texture(GraphicComponent & component, const string & filepath, const string & atlas = "");
  Texture() = delete;
  Texture(Texture & other) = delete;
  virtual ~Texture();

  bool loadFromFile(const string & filepath, const string & atlas = "");
  bool hasImage();
  void setPosition(const Vector3 & p);
//  const Vector3 & getPosition();

  virtual void paint() override;
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
  unique_ptr< Context > _data;
};


} // end namespace Engine

#endif // TEXTURE_HPP
