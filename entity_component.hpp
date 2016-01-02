/**
  * base entities data component
  */

#ifndef ENTITY_COMPONENT_HPP
#define ENTITY_COMPONENT_HPP

#include "basic_types.hpp"

namespace Engine
{

typedef int EntityID;

struct EntityComponent
{
  EntityID entityId;

  // kind of space 2D/3D
  eSpace kind;

  // position
  // absolute in pixels (float values)
  // can contain z-order
  Vector3 position;

  // rotation
  // values between 0.0 - 1.0  (= 0 - 360)
  Rotation3 rotation;

  // scales
  // <1.0 : smaller | > 1.0 : larger
  // this attribure is shared by physics and
  // graphics, and might have application
  // to the entity behaviour as well
  Vector3 scale;

  // whether or not this entity is active
  bool isActive;
};

} // end namespace Engine

#endif // ENTITY_COMPONENT_HPP
