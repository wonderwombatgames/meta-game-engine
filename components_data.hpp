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

struct TransformationsComponent
{
  // kind of transforation 2D/3D
  eTransformation kind;

  // position
  // values between 0.0 - 1.0 (in relation to world size)
  Vector3 position;

  // size
  // values between 0.0 - 1.0 (in relation to world size)
  Vector3 size;

  // rotation
  // values between 0.0 - 1.0  (= 0 - 360)
  Rotation rotation;

  // scales
  // <1.0 : smaller | > 1.0 : larger
  Vector3 scale;

  // defines the anchor within the boudaries
  // also used by phisics = center of mass
  // and rendering = drawing pivot
  // values between 0.0 - 1.0 (in relation to entity size)
  Vector3 anchor;

  // entity boundaries
  // used by phisics as well.
  // values between 0.0 - 1.0 (in relation to entity size)
  Boundary boundary;
};

struct PhysicsAndCollisionsComponent
{
  // values between 0.0 - 1.0 (in relation to world size) / t / t
  Vector3 linVelocity;
  Vector3 linAcceleration;
  Vector3 angVelocity;
  Vector3 angAcceleration;
  // arbritary value in relation to other entities
  float mass;
  // values between 0.0 - 1.0
  float elasticity;
};

struct GraphicsAndAnimationsComponent
{
  AtlasData * atlas;
  TextureData * texture;
  GraphicEffectsData * effects;
  ShaderData * shader;
  ParticlesEmitterData * emitter;
  Colour colour;
  Colour tint;
  // AlphaMode ???
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
{};

struct LogicAndStatesComponent
{};



} // end namespace Engine

#endif // COMPENTS_DATA_HPP
