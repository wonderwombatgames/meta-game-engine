/**
  * base entities data component
  */

#ifndef ENTITY_COMPONENT_HPP
#define ENTITY_COMPONENT_HPP

#include "utils_types.hpp"

namespace W2E
{
using namespace Utils;

namespace Component
{

  // forward declaration
  struct TransformPod;


  struct EntityPod
  {
    // entity unique identifier
    EntityID entityId;

    // define to what class entity belongs
    TypeID typeId;

    // whether or not this entity is active
    bool isActive;

    // pointer to transform pod
    TransformPod * transform;

  };

} // namespace Component

} // end namespace W2E

#endif // ENTITY_COMPONENT_HPP
