/**
  * structs containing component data
  *
  */

#ifndef COMPONENT_PHYSICS_HPP
#define COMPONENT_PHYSICS_HPP

#include "utils/types.hpp"

namespace W2E
{
using namespace Utils;

namespace Component
{

// forward declaration
class PhysicsInterface;
struct TransformPod;

struct PhysicsPod
{
  // values between 0.0 - 1.0 (in relation to world size) / t / t
  Vector3 linVelocity;
  Vector3 linAcceleration;
  Rotation3 angVelocity;
  Rotation3 angAcceleration;

  // body
  Bound bodyContour;

  // defines the center of the body
  // values between 0.0 - 1.0 (in relation to entity Contour)
  Vector3 centerOfMass;

  // arbritary value in relation to other entities
  f32 mass;

  // values between 0.0 - 1.0
  f32 elasticity;

  // reference to transform data component
  TransformPod* transformData = nullptr;

  // physics component data pointer
  PhysicsInterface* resource = nullptr;
};

class PhysicsInterface
{
public:
  PhysicsInterface() {}
  virtual ~PhysicsInterface() {}
  virtual void step(const PhysicsPod& component, const TransformPod& transformData) = 0;
};

} // namespace Component

} // end namespace W2E

#endif // COMPONENT_PHYSICS_HPP
