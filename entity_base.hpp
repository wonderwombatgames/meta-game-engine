/**
  * entities manager contains all entities
  * entities are just containers for components
  *
  *
  */

#ifndef ENTITY_BASE_HPP
#define ENTITY_BASE_HPP

#include "utils_types.hpp"
#include "component_entity.hpp"

namespace W2E
{

namespace System
{
  class SystemsInterface;
}

using namespace Utils;


class EntityBase
{
public:
  friend class EntitiesManager;
  // friend class SystemsInterface;

  // ctors and dtor
  EntityBase(EntityID id);
  EntityBase() = delete;
  EntityBase(EntityBase & other) = delete;
  virtual ~EntityBase();

  ErrorCode suspend();
  ErrorCode resume();
  ErrorCode destroy(bool mustDestroy = true);
  bool willDestroy();
  bool isActive() const;
  // add one more component to the entity
  EntityID registerIntoSystem(System::SystemsInterface & system);
  // remove one more component to the entity
  ErrorCode deregisterFromSystem(System::SystemsInterface & system);
  // verify if entity has component
  bool containedInSystem(System::SystemsInterface * system);

protected:
  // can be used to register components into systems from constructor
  virtual void setUpComponents() {};

  // tear down (de register) components from systems
  void tearDownComponents();


  // data
  bool _destroy;

  // set holds the systems to wich this entity was added
  Set< System::SystemsInterface * > _componentSystems;

  // this POD is to be used by the component systems
  Component::EntityPod _entityData;
};


inline bool EntityBase::isActive() const
{
  return this->_entityData.isActive;
}

inline ErrorCode EntityBase::destroy(bool mustDestroy)
{
  this->_destroy = mustDestroy;
  return (this->_destroy)?NO_ERROR:UNKNOWN_ERROR;
}

inline bool EntityBase::willDestroy()
{
  return this->_destroy;
}

inline bool EntityBase::containedInSystem(System::SystemsInterface * system)
{
  return (0 < this->_componentSystems.count(system));
}


} // end namespace W2E

#endif // ENTITY_BASE_HPP
