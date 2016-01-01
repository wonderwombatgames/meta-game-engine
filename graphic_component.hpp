/**
  * base entities data component
  */

#ifndef GRAPHIC_COMPONENT_HPP
#define GRAPHIC_COMPONENT_HPP

#include <cstdint>

#include "entity_component.hpp"

// forward declaration
class Texture;
//class GraphicEffectsData;
//class ShaderData;
//class ParticlesEmitterData;

namespace Engine
{

struct GraphicComponent
{
  // reference to entity data component
  EntityComponent * entityData;

  // texture data
  Texture * texture;
  Vector3 textureSize;
  unsigned short animationFrame;

  // defines the anchor within the boudaries
  // and rendering = drawing pivot
  // values between 0.0 - 1.0 (in relation to entity size | UV)
  Vector3 anchor;

  // colour parameters
  Colour colourMode;
  ColourComponent alphaMode;
  BlendingMode blendingMode;

  // special components
  //GraphicEffectsData * effects;
  //ShaderData * shader;
  //ParticlesEmitterData * emitter;
};

} // end namespace Engine

#endif // GRAPHIC_COMPONENT_HPP
