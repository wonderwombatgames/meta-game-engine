/**
  * base entities data component
  */

#ifndef COMPONENT_ENTITY_HPP
#define COMPONENT_ENTITY_HPP

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
  bool isActive = true;

  // pointer to transform pod
  TransformPod* transform = nullptr;
};

} // namespace Component

} // end namespace W2E

#endif // COMPONENT_ENTITY_HPP
