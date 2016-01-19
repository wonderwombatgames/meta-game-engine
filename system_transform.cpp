/**
  *
  */

#include "system_transform.hpp"

namespace W2E
{
using namespace std;
namespace System
{

  Transform::Transform()
      :SystemsInterface("Transform")
  {
  }

  Transform::Transform(const char * name)
      :SystemsInterface(name)
  {
  }

  Transform::~Transform()
  {
  }

  void Transform::insert(Component::EntityPod & entity)
  {
    Component::TransformPod pod;
    this->_components.emplace(entity.entityId, pod);
    entity.transform = &(this->_components[entity.entityId]);
    entity.transform->position = {{ 0.0f, 0.0f, 0.0f }};
    entity.transform->rotation = {{ 0.0f, 0.0f, 0.0f }};
    entity.transform->scale    = {{ 1.0f, 1.0f, 1.0f }};
  }
  void Transform::remove(const Component::EntityPod & entity)
  {
    auto it = this->_components.find(entity.entityId);
    if (it != this->_components.end())
    {
      this->_components.erase(it);
    }
  }
  void Transform::tick(TimeDim delta)
  {

  }

} // end namespace System;


} // end namespace W2E
