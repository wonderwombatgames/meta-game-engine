/**
  *
  */

#include "system_input.hpp"

namespace W2E
{

namespace System
{

Input::Input()
    : SystemsInterface{"Input"}
    , components_{}
    , resources_{}
{
}

Input::Input(const char* name)
    : SystemsInterface{name}
    , components_{}
    , resources_{}
{
}

Input::~Input() {}

void Input::insert(Component::EntityPod& entity)
{
  Component::InputPod pod{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0x0000, {0}, nullptr};
  this->components_.emplace(entity.entityId, pod);
  // TODO: init the inputPod to 0.0
}
void Input::remove(const Component::EntityPod& entity)
{
  auto it = this->components_.find(entity.entityId);
  if(it != this->components_.end())
  {
    this->components_.erase(it);
  }
}
void Input::tick(TimeDim delta) {}

ComponentBinderPtr Input::getComponentBinder(ResourceID resourceId)
{
  using InputComponentBinder = ComponentBinder< Component::InputInterface, InputComponents >;
  ComponentBinderPtr retVal;
  auto it = resources_.find(resourceId);
  if(it != resources_.end())
  {
    retVal.reset(new InputComponentBinder(this, it->second.get(), &components_));
  }
  return retVal;
}

} // end namespace System;

} // end namespace W2E
