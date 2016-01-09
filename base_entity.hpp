/**
  * entities manager contains all entities
  * entities are just containers to components
  *
  *
  */

#ifndef BASE_ENTITY_HPP
#define BASE_ENTITY_HPP

#include <string>
#include <set>
#include "system_interface.hpp"


namespace Engine
{
using namespace std;
using namespace System;

class BaseEntity
{
public:
  friend class EntitiesManager;
  // ctors and dtor
  BaseEntity(EntityID id, const char * name);
  BaseEntity() = delete;
  BaseEntity(BaseEntity & other) = delete;
  virtual ~BaseEntity();

  const string & getName() const;
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
  string _name;

  set< SystemsInterface * > _componentSystems;
  // this POD is to be used by the component systems
  Component::EntityPod _entityData;
  // contains spatial information
  Component::TransformPod * _transformData;
};


inline const string & BaseEntity::getName() const
{
  return this->_name;
}

inline bool BaseEntity::isActive() const
{
  return this->_entityData.isActive;
}

inline void BaseEntity::destroy(bool mustDestroy)
{
  this->_destroy = mustDestroy;
}

inline bool BaseEntity::destroy()
{
  return this->_destroy;
}

inline bool BaseEntity::hasComponent(SystemsInterface & system)
{
  return (0 < this->_componentSystems.count(&system));
}


} // end namespace Engine

#endif // BASE_ENTITY_HPP
