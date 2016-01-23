/**
  *
  */

#ifndef SYSTEM_TRANSFORM_HPP
#define SYSTEM_TRANSFORM_HPP

#include "system_interface.hpp"
#include "component_transform.hpp"

namespace W2E
{
using namespace std;

namespace System
{

using TransformComponents = HashMap< EntityID, Component::TransformPod >;

class Transform : public SystemsInterface
{
public:
  Transform();
  Transform(const char* name);
  Transform(Transform& other) = delete;
  virtual ~Transform();

protected:
  virtual void insert(Component::EntityPod& entity) override;
  virtual void remove(const Component::EntityPod& entity) override;
  virtual void tick(TimeDim delta) override;
  virtual ComponentBinderPtr getComponentBinder(ResourceID resourceId) override;

  TransformComponents components_;
};

} // end namespace System

} // end namespace W2E

#endif // SYSTEM_TRANSFORM_HPP
