/**
  * entities manager contains all entities
  * entities are just containers to components
  *
  *
  */

#ifndef ENTITIES_MANAGER_HPP
#define ENTITIES_MANAGER_HPP

#include <string>
#include <unordered_map>
#include <set>
#include <deque>
#include <memory>
#include "system_interface.hpp"


namespace Engine
{
using namespace std;
using namespace System;


class EntitiesManager
{
public:

  class IEntity
  {
  public:
    // ctors and dtor
    IEntity(EntityID id, const char * name);
    IEntity() = delete;
    IEntity(IEntity & other) = delete;
    virtual ~IEntity();

    const string & getName() const;
    bool isActive() const;
    bool suspend();
    bool resume();
    void destroy(bool mustDestroy);
    bool destroy();
    bool hasComponent(BaseInterface & system);

  protected:
    // can be used to register components into systems from constructor
    virtual void setUpComponents() {};

    // tear down (de register) components from systems
    void tearDownComponents();

    // add one more component to the entity
    void addComponent(BaseInterface & system);

    // data
    bool _destroy;
    string _name;

    set< BaseInterface * > _componentSystems;
    // this POD is to be used by the component systems
    Component::EntityPod _entityData;
    // contains spatial information
    Component::TransformPod _transformData;
  };

  static EntitiesManager * instance();
  int count();

  // entity related methods
  template< class EntityType = IEntity >
  int  createEntity(const char * name);
  bool destroyEntity(EntityID entityId);
  bool suspendEntity(EntityID entityId);
  bool resumeEntity(EntityID entityId);
  bool isEntityActive(EntityID entityId);
  const string * lookUpentityName(EntityID entityId);
  EntityID lookUpEntityId(const string& name);
  int  refreshEntities();

  // component related methods
  bool addComponent(EntityID entitityId, BaseInterface & system);

protected:
  enum {  MAX_ENTITIES_AMOUNT = 10000  };

  // creates a new random entity ID < MAX_ENTITIES_AMOUNT
  EntityID newId();

  // CTOR
  EntitiesManager() : _count(0) {};

  // private typedefs
  typedef shared_ptr<IManagedEntity> IEntityPtr;
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
  EntityType * entity = new EntityType(id, name);
  entity->setUpComponents();
  this->_entities[id].reset(entity);
  this->_lookUp[entity->getName()] = id;
  this->refreshEntities();
  return id;
}

inline int EntitiesManager::count()
{
  return this->_count;
}

inline const string & EntitiesManager::IEntity::getName() const
{
  return this->_name;
}

inline bool EntitiesManager::IEntity::isActive() const
{
  return this->_entityData.isActive;
}

inline void EntitiesManager::IEntity::destroy(bool mustDestroy)
{
  this->_destroy = mustDestroy;
}

inline bool EntitiesManager::IEntity::destroy()
{
  return this->_destroy;
}

inline bool EntitiesManager::IEntity::hasComponent(BaseInterface & system)
{
  return (0 < this->_componentSystems.count(&system));
}

// un-nesting entity class
class IManagedEntity : public EntitiesManager::IEntity
{
  friend class EntitiesManager;
public:
  virtual ~IManagedEntity(){}
  IManagedEntity() = delete;
  IManagedEntity(IManagedEntity & other) = delete;
  IManagedEntity(EntityID id, const char * name = nullptr) :
    EntitiesManager::IEntity(id, name)
    {}
};

} // end namespace Engine

#endif // ENTITIES_MANAGER_HPP
