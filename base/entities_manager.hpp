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

namespace W2E
{

using namespace System;

class EntitiesManager
{
public:
  CLASS_METHOD EntitiesManager* instance();
  int count();

  // entity related methods
  template < class EntityType = EntityBase >
  EntityID createEntity(const char* name);
  EntityRegistrarInterface* registrar(EntityID entityId);
  // const String * lookUpEntityName(EntityID entityId);
  EntityID lookUpEntityId(const String& name);
  int refreshEntities();

protected:
  enum
  {
    MAXENTITIES_AMOUNT_ = 99999
  };

  // CTOR
  EntitiesManager()
      : count_{0}
      , entities_{}
      , lookUp_{}
  {
  }

  // private type defs
  using IEntityPtr = SharedPtr< EntityBase >;
  using Entities = HashMap< EntityID, IEntityPtr >;
  using EntitiesLookUp = HashMap< String, EntityID >;

  // data
  int count_;
  Entities entities_;
  EntitiesLookUp lookUp_;
};

template < class EntityType >
inline EntityID EntitiesManager::createEntity(const char* name)
{
  EntityID id = rndId();
  EntityType* entity = new EntityType(id);
  entity->setUpComponents();
  this->entities_[id].reset(entity);
  this->lookUp_[name] = id;
  this->refreshEntities();
  return id;
}

inline int EntitiesManager::count() { return this->count_; }

} // end namespace W2E

#endif // ENTITIES_MANAGER_HPP
