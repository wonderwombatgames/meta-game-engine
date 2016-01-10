/**
  * entities manager contains all entities
  * entities are just containers for components
  *
  *
  */

#ifndef ENTITY_BASE_HPP
#define ENTITY_BASE_HPP

#include <set>
#include "system_interface.hpp"


namespace Engine
{
using namespace std;
using namespace System;

class EntityBase
{
public:
  friend class EntitiesManager;
  // ctors and dtor
  EntityBase(EntityID id);
  EntityBase() = delete;
  EntityBase(EntityBase & other) = delete;
  virtual ~EntityBase();

  bool isActive() const;
  bool suspend();
  bool resume();
  void destroy(bool mustDestroy);
  bool destroy();
  bool hasComponent(SystemsInterface & system);

protected:
  // can be used to register components into systems from constructor
  virtual void setUpComponents() {};

  // tear down (de register) components from systems
  void tearDownComponents();

  // add one more component to the entity
  void addComponent(SystemsInterface & system);

  // data
  bool _destroy;

  set< SystemsInterface * > _componentSystems;
  // this POD is to be used by the component systems
  Component::EntityPod _entityData;
  // contains spatial information
  Component::TransformPod * _transformData;
};


inline bool EntityBase::isActive() const
{
  return this->_entityData.isActive;
}

inline void EntityBase::destroy(bool mustDestroy)
{
  this->_destroy = mustDestroy;
}

inline bool EntityBase::destroy()
{
  return this->_destroy;
}

inline bool EntityBase::hasComponent(SystemsInterface & system)
{
  return (0 < this->_componentSystems.count(&system));
}


} // end namespace Engine

#endif // ENTITY_BASE_HPP
