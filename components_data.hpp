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
class MusicData;
class SoundFxData;

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

  // defines the center of the body
  // values between 0.0 - 1.0 (in relation to entity Contour)
  Vector3 centerOfMass;

  // values between 0.0 - 1.0
  float elasticity;

  // body
  Boundary bodyContour;
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
