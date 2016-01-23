/**
  *
  */

#ifndef SYSTEM_INPUT_HPP
#define SYSTEM_INPUT_HPP

#include "system_interface.hpp"
#include "component_input.hpp"

namespace W2E
{

namespace System
{

using InputComponents = HashMap< EntityID, Component::InputPod >;
using InputResourcePtr = SharedPtr< Component::InputInterface >;
using InputResources = HashMap< ResourceID, InputResourcePtr >;

class Input : public SystemsInterface
{
public:
  Input();
  Input(const char* name);
  Input(Input& other) = delete;
  virtual ~Input();

protected:
  virtual void insert(Component::EntityPod& entity) override;
  virtual void remove(const Component::EntityPod& entity) override;
  virtual void tick(TimeDim delta) override;
  virtual ComponentBinderPtr getComponentBinder(ResourceID resourceId) override;

  InputComponents components_;
  InputResources resources_;
};

} // end namespace System

} // end namespace W2E

#endif // SYSTEM_INPUT_HPP
