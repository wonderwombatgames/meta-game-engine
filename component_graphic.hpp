/**
  * base entities data component
  */

#ifndef GRAPHIC_COMPONENT_HPP
#define GRAPHIC_COMPONENT_HPP

#include "component_transform.hpp"

namespace Engine
{
using namespace Utils;

// forward declaration
//class IGraphicEffectsData;
//class IShaderData;
//class IParticlesEmitterData;

class IGraphic
{
public:
  IGraphic(){}
  virtual ~IGraphic(){}
  virtual void paint(const Vector3 & offset = {0.0f, 0.0f, 0.0f}) = 0;
};

struct GraphicComponent
{
  // reference to entity data component
  TransformComponent * transformData;

  // defines the anchor within the boudaries
  // values between 0.0 - 1.0 (in relation to entity size | UV)
  Vector3 anchor;

  // size - between 0.0 - 1.0 (in relation to viewport size)
  Dimension3 textureSize;

  // texture data pointer
  IGraphic * texture;

  // nth frame in within the texture atlas
  unsigned short animationFrame;

  // colour parameters
  Colour colourTint;
  ColourComponent alphaMode;
  BlendingMode blendingMode;

  // whether to show the entity or not
  bool isVisible;

  // special components
  //GraphicEffectsData * effects;
  //ShaderData * shader;
  //ParticlesEmitterData * emitter;
};

} // end namespace Engine

#endif // GRAPHIC_COMPONENT_HPP
