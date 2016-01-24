/**
  * structs containing component data
  *
  */

#ifndef PHYSICS_COMPONENT_HPP
#define PHYSICS_COMPONENT_HPP

#include "utils_types.hpp"

namespace W2E
{
using namespace Utils;

namespace Component
{

// forward declaration
class InputInterface;

struct InputPod
{
  // values between -1.0 and +1.0
  f32 axis[6];

  // values bitmask of up to 16 buttons
  u16 buttons;

  // keyboard buffer
  char keysBuffer[32];

  // graphic element data pointer
  InputInterface* resource;
};

class InputInterface
{
public:
  InputInterface() {}
  virtual ~InputInterface() {}
  virtual void poll(const InputPod& component) = 0;
};

} // namespace Component

} // end namespace W2E

#endif // PHYSICS_COMPONENT_HPP
