/**
  * structs containing component data
  * they are POD only
  *
  *
  */

#ifndef COMPENTS_DATA_HPP
#define COMPENTS_DATA_HPP

#include "data_utils.hpp"

// forward declaration
class AtlasData;
class TextureData;
class MusicData;
class SoundFxData;
class GraphicEffectsData;
class ShaderData;
class ParticlesEmitterData;

namespace Engine
{

struct PhysicsAndCollisionsComponent
{
  // values between 0.0 - 1.0 (in relation to world size) / t / t
  Vector3 linVelocity;
  Vector3 linAcceleration;
  Rotation3 angVelocity;
  Rotation3 angAcceleration;

  // arbritary value in relation to other entities
  float mass;

  // values between 0.0 - 1.0
  float elasticity;

  // body
  Boundary bodyContour;
};

struct GraphicsAndAnimationsComponent
{
  // defines the anchor within the boudaries
  // also used by phisics = center of mass
  // and rendering = drawing pivot
  // values between 0.0 - 1.0 (in relation to entity size)
  Vector3 anchor;

  // entity boundaries
  // values between 0.0 - 1.0 (in relation to world size and the anchor)
  BoxBoundary clipRectangle;

  // colour parameters
  Colour colour;
  Colour tint;
  // AlphaMode ???

  // texture data
  unsigned int animationFrame;
  TextureData * texture;
  AtlasData * atlas;

  // special components
  GraphicEffectsData * effects;
  ShaderData * shader;
  ParticlesEmitterData * emitter;

};

struct SoundEffectsComponent
{
  MusicData * music;
  // values between 0.0 - 1.0
  float musicVolume;
  eSoundState musicState;
  SoundFxData * soudFx;
  // values between 0.0 - 1.0
  float soundFxVolume;
  eSoundState soundFxState;
};

struct InputAndControllersComponent
{

};

struct LogicAndStatesComponent
{

};


} // end namespace Engine

#endif // COMPENTS_DATA_HPP
