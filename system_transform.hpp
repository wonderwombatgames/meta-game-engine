/**
  *
  */

#ifndef SYSTEM_TRANSFORM_HPP
#define SYSTEM_TRANSFORM_HPP

#include <unordered_map>

#include "system_interface.hpp"
#include "component_transform.hpp"

namespace Engine
{
using namespace std;

namespace System
{

  class Transform : public SystemsInterface
  {
  public:
    Transform();
    Transform(const char * name);
    Transform(Transform & other) = delete;
    virtual ~Transform();

  protected:
    virtual void add(const Component::EntityPod & entity, Component::TransformPod * transform) override;
    virtual void del(const Component::EntityPod & entity) override;
    virtual void tick(TimeDimension delta) override;

    unordered_map< EntityID, Component::TransformPod > _components;
  };

} // end namespace System


} // end namespace Engine

#endif // SYSTEM_TRANSFORM_HPP
