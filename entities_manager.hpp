/**
  * entities manager contains all entities
  * entities are just containers to components
  *
  *
  */

#ifndef ENTITIES_MANAGER_HPP
#define ENTITIES_MANAGER_HPP

#include <memory>

#include "entity_base.hpp"

namespace Engine
{
using namespace std;
using namespace System;

class EntitiesManager
{
public:


  static EntitiesManager * instance();
  int count();

  // entity related methods
  template< class EntityType = EntityBase >
  EntityID  createEntity(const char * name);
  EntityBase & entity(EntityID entityId);

  // bool destroyEntity(EntityID entityId);
  // bool suspendEntity(EntityID entityId);
  // bool resumeEntity(EntityID entityId);
  // bool isEntityActive(EntityID entityId);
  // bool addComponent(EntityID entitityId, SystemsInterface & system);
  //const String * lookUpEntityName(EntityID entityId);
  EntityID lookUpEntityId(const String& name);
  int  refreshEntities();

protected:
  enum {  MAX_ENTITIES_AMOUNT = 99999  };

  // CTOR
  EntitiesManager() : _count(0) {};

  // private type defs
  using IEntityPtr = SharedPtr<EntityBase>;
  using Entities = HashMap<EntityID, IEntityPtr>;
  using EntitiesLookUp = HashMap<String, EntityID>;

  // data
  int _count;
  Entities _entities;
  EntitiesLookUp _lookUp;
};

template< class EntityType >
inline EntityID EntitiesManager::createEntity(const char * name)
{
  EntityID id = rndId();
  EntityType * entity = new EntityType(id);
  entity->setUpComponents();
  this->_entities[id].reset(entity);
  this->_lookUp[name] = id;
  this->refreshEntities();
  return id;
}

inline int EntitiesManager::count()
{
  return this->_count;
}


} // end namespace Engine

#endif // ENTITIES_MANAGER_HPP
