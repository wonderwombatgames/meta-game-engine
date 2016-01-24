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

// FIXME: this system needs to support multiple componts per entity!!!
// TODO: can the other system support more than one component as well???
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

  template < class ComponentType >
  ResourceID loadComponent();

protected:
  virtual void insert(Component::EntityPod& entity) override;
  virtual void remove(const Component::EntityPod& entity) override;
  virtual void tick(TimeDim delta) override;
  virtual ComponentBinderPtr getComponentBinder(ResourceID resourceId) override;

  InputComponents components_;
  InputResources resources_;
};

template < typename ComponentType >
ResourceID Input::loadComponent()
{
  ResourceID resCounter = seqId();
  this->resources_.emplace(resCounter, std::make_shared< ComponentType >());

  return resCounter;
}

} // end namespace System

} // end namespace W2E

#endif // SYSTEM_INPUT_HPP
