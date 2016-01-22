/**
  * base entities data component
  */

#ifndef COMPONENT_CONTROLLER_HPP
#define COMPONENT_CONTROLLER_HPP

#include "utils_types.hpp"

namespace W2E
{
using namespace Utils;

namespace Component
{

struct ControllerPod
{
  // entity unique identifier
  EntityID entityId;

  // whether or not this entity is active
  bool isActive = true;
};

} // namespace Component

} // end namespace W2E

#endif // COMPONENT_CONTROLLER_HPP
