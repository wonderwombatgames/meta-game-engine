/**
  * base system interface
  * other systems will inherity from this class
  *
  *
  */

#ifndef COMPONENT_BINDER_HPP
#define COMPONENT_BINDER_HPP

#include "utils_types.hpp"

namespace W2E
{
using namespace Utils;

// forward declarations
namespace System
{
class SystemsInterface;
class ComponentBinderInterface;
}

// interface implemented by base class to support registering components
class EntityRegistrarInterface
{
private:
  friend class System::ComponentBinderInterface;
  EntityID registerIntoSystem(System::SystemsInterface& system)
  {
    return this->registerIntoSystem_(system);
  }

protected:
  EntityRegistrarInterface() {}
  virtual ~EntityRegistrarInterface() {}

  // add one more component to the entity
  virtual EntityID registerIntoSystem_(System::SystemsInterface& system) = 0;
};

namespace System
{

// interface used to provide access to register a component to an entity and into its system
class ComponentBinderInterface
{
public:
  virtual ~ComponentBinderInterface() {}
  virtual ErrorCode toEntity(EntityRegistrarInterface* er) = 0;

protected:
  EntityID registerIntoSystem_(SystemsInterface* sys, EntityRegistrarInterface* er)
  {
    if(sys != nullptr && er != nullptr)
    {
      return er->registerIntoSystem(*sys);
    }
    return InvalidID;
  }
};

// template concrete class to provide binders to each system
// systems will return the resolved template pointing to their own data types
template < class Resource, class Container >
class ComponentBinder : public ComponentBinderInterface
{
public:
  // virtual dtor
  virtual ~ComponentBinder() {}
  // Constructor
  ComponentBinder(SystemsInterface* system, Resource* resource, Container* components);

  // bind this resource to entity
  virtual ErrorCode toEntity(EntityRegistrarInterface* er);

private:
  SystemsInterface* system_;
  Resource* resource_;
  Container* components_;
};

template < class Resource, class Container >
ComponentBinder< Resource, Container >::ComponentBinder(SystemsInterface* system,
                                                        Resource* resource,
                                                        Container* components)
    : system_(system)
    , resource_(resource)
    , components_(components)
{
}

template < class Resource, class Container >
ErrorCode ComponentBinder< Resource, Container >::toEntity(EntityRegistrarInterface* er)
{
  if(system_ != nullptr && er != nullptr)
  {
    EntityID id = this->registerIntoSystem_(system_, er);

    auto it = this->components_->find(id);
    if(it != this->components_->end())
    {
      it->second.resource = resource_;
      return NO_ERROR;
    }
  }
  return UNKNOWN_ERROR;
}

using ComponentBinderPtr = SharedPtr< ComponentBinderInterface >;

} // end namespace System

} // end namespace W2E

#endif // COMPONENT_BINDER_HPP
