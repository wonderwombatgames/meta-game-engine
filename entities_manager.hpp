/**
  * entities manager contains all entities
  * entities are just containers to components
  *
  *
  */

#ifndef ENTITIES_MANAGER_HPP
#define ENTITIES_MANAGER_HPP

#include <unordered_map>
#include <memory>

#include "system_interface.hpp"
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
  bool destroyEntity(EntityID entityId);
  bool suspendEntity(EntityID entityId);
  bool resumeEntity(EntityID entityId);
  bool isEntityActive(EntityID entityId);
  //const string * lookUpEntityName(EntityID entityId);
  EntityID lookUpEntityId(const string& name);
  int  refreshEntities();

  // component related methods
  bool addComponent(EntityID entitityId, SystemsInterface & system);

protected:
  enum {  MAX_ENTITIES_AMOUNT = 99999  };

  // creates a new random entity ID < MAX_ENTITIES_AMOUNT
  EntityID newId();

  // CTOR
  EntitiesManager() : _count(0) {};

  // private typedefs
  typedef shared_ptr<EntityBase> IEntityPtr;
  typedef unordered_map<EntityID, IEntityPtr> Entities;
  typedef unordered_map<string, EntityID> EntitiesLookUp;

  // data
  int _count;
  Entities _entities;
  EntitiesLookUp _lookUp;
};

template< class EntityType >
inline EntityID EntitiesManager::createEntity(const char * name)
{
  EntityID id = this->newId();
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
