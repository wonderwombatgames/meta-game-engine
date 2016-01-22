/**
  *
  */

#include "system_controller.hpp"

namespace W2E
{

namespace System
{

Controller::Controller()
    : SystemsInterface("Controller")
{
}

Controller::Controller(const char* name)
    : SystemsInterface(name)
{
}

Controller::~Controller() {}

void Controller::insert(Component::EntityPod& entity)
{
  Component::ControllerPod pod;
  this->components_.emplace(entity.entityId, pod);
}
void Controller::remove(const Component::EntityPod& entity)
{
  auto it = this->components_.find(entity.entityId);
  if(it != this->components_.end())
  {
    this->components_.erase(it);
  }
}
void Controller::tick(TimeDim delta) {}

} // end namespace System;

} // end namespace W2E
