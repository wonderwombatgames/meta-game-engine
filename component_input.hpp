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

struct Event
{
  u32 type;
  u32 value;
};

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
  virtual void poll(const Event& event, InputPod& component) = 0;
};

class WasdInput : public InputInterface
{
public:
  WasdInput() {}
  virtual ~WasdInput() {}
  virtual void poll(const Event& event, InputPod& component) override;
};

class ArrowsInput : public InputInterface
{
public:
  ArrowsInput() {}
  virtual ~ArrowsInput() {}
  virtual void poll(const Event& event, InputPod& component) override;
};

// class XInput : public InputInterface
//{
// public:
//  XInput() {}
//  virtual ~XInput() {}
//  virtual void poll(const Event& event, InputPod& component) override;
//};
//
// class NumPadInput : public InputInterface
//{
// public:
//  NumPadInput() {}
//  virtual ~NumPadInput() {}
//  virtual void poll(const Event& event, InputPod& component) override;
//};

} // namespace Component

} // end namespace W2E

#endif // PHYSICS_COMPONENT_HPP
