/**
  *
  */

#include "system_transform.hpp"

namespace W2E
{

namespace System
{

Transform::Transform()
    : SystemsInterface("Transform")
{
}

Transform::Transform(const char* name)
    : SystemsInterface(name)
{
}

Transform::~Transform() {}

void Transform::insert(Component::EntityPod& entity)
{
  Component::TransformPod pod;
  this->components_.emplace(entity.entityId, pod);
  entity.transform = &(this->components_[entity.entityId]);
  entity.transform->position = {{0.0f, 0.0f, 0.0f}};
  entity.transform->rotation = {{0.0f, 0.0f, 0.0f}};
  entity.transform->scale = {{1.0f, 1.0f, 1.0f}};
}
void Transform::remove(const Component::EntityPod& entity)
{
  auto it = this->components_.find(entity.entityId);
  if(it != this->components_.end())
  {
    this->components_.erase(it);
  }
}
void Transform::tick(TimeDim delta) {}

ComponentBinderPtr Transform::getComponentBinder(ResourceID resourceId)
{
  using TransformComponentBinder = ComponentBinder< void, TransformComponents >;
  ComponentBinderPtr retVal;
  retVal.reset(new TransformComponentBinder(this, nullptr, &components_));
  return retVal;
}

} // end namespace System;

} // end namespace W2E
