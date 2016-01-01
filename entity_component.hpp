/**
  * base entities data component
  */

#ifndef ENTITY_COMPONENT_HPP
#define ENTITY_COMPONENT_HPP

#include "data_utils.hpp"

namespace Engine
{

typedef int EntityID;

struct EntityComponent
{
  EntityID entityId;

  // kind of space 2D/3D
  eSpace kind;

  // position
  // values between 0.0 - 1.0 (in relation to world size)
  Vector3 position;

  // rotation
  // values between 0.0 - 1.0  (= 0 - 360)
  Rotation3 rotation;

  // scales
  // <1.0 : smaller | > 1.0 : larger
  // this attribure i chared by physics and
  // graphics, but might have application
  // to the entity behaviour
  Vector3 scale;

  // whether or not this entity is active
  bool isActive;
};

} // end namespace Engine

#endif // ENTITY_COMPONENT_HPP
