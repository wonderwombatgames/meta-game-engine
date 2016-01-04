/**
  * structs containing component data
  *
  */

#ifndef PHYSICS_COMPONENT_HPP
#define PHYSICS_COMPONENT_HPP

#include "utils_types.hpp"


namespace Engine
{
using namespace Utils;

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
  Bound bodyContour;
};

} // end namespace Engine

#endif // PHYSICS_COMPONENT_HPP
