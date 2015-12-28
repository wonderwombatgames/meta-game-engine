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
#include "data_utils.hpp"

namespace Engine
{
using namespace std;

//
struct EntitiesBaseComponent
{
  int entityId;

  // kind of space 2D/3D
  eSpace kind;

  // position
  // values between 0.0 - 1.0 (in relation to world size)
  Vector3 position;

  // rotation
  // values between 0.0 - 1.0  (= 0 - 360)
  Rotation3 rotation;

  // scales
  // <1.0 : smaller | > 1.0 : larger
  Vector3 scale;
};

//
class EntitiesManager
{
public:

  class IEntity
  {
  public:
    // this POD is to be used by the component systems
    EntitiesBaseComponent _entityData;

    virtual ~IEntity();

    IEntity(int id, const char * name);
    IEntity() = delete;
    IEntity(IEntity & other) = delete;


    // this methods are mostly used by the component systems
    bool suspend();

    bool resume();

    bool isActive()
    {  return this->_isActive;  }

    void destroy()
    {  this->_mustDestroy = true;  }

    bool mustDestroy()
    {  return this->_mustDestroy;  }

    bool isInSystem(ISystem & system)
    {  return (0 < this->_componentSystems.count(&system));  }

    int getId()
    {  return this->_entityData.entityId;  }

    const string & getName()
    {  return this->_name; }

  protected:
    bool _isActive;
    bool _mustDestroy;
    string _name;
    set< ISystem * > _componentSystems;

    // can be overriden on each entityType
    // register components into systems during constructor exec
    virtual void setUpComponents() {};

    // tear down (de register) components from systems
    void tearDownComponents();

    // add one more component to the entity
    void addComponent(ISystem & system);

  };

  // singleton access
  static EntitiesManager * instance();

  int size()
  {  return this->_size;  }

  // entity related methods
  template< class EntityType = IEntity >
  int createEntity(const char * name);

  bool destroyEntity(int entityId);

  bool suspendEntity(int entityId);

  bool resumeEntity(int entityId);

  int refreshEntities();

  // component related methods
  bool addComponent(int entitityId, ISystem & system);

  void updateComponents(ISystem & system);

protected:
  enum {  MAX_ENTITIES_AMOUNT = 10000  };

  // creates a new random entity ID < MAX_ENTITIES_AMOUNT
  int newId();

  // CTOR
  EntitiesManager() : _size(0) {};

  typedef shared_ptr<IManagedEntity> IEntityPtr;
  typedef unordered_map<int, IEntityPtr> Entities;
  typedef unordered_map<string, int> EntitiesLookUp;

  int _size;
  Entities _entities;
  EntitiesLookUp _lookUp;
  deque<int> _activeEntities;
};

inline EntitiesManager::IEntity::IEntity(int id, const char * name):
  _isActive(true),
  _mustDestroy(false),
  _name(name)
  {
    // defaults
    this->_entityData.entityId = id;
    this->_entityData.kind = SPACE_2D;
  };

template< class EntityType >
inline int EntitiesManager::createEntity(const char * name)
{
  int id = this->newId();
  EntityType * entity = new EntityType(id, name);
  entity->setUpComponents();
  this->_entities[id].reset(entity);
  this->_lookUp[entity->getName()] = id;
  this->_size = this->refreshEntities();
  return id;
}

// un-nesting entity class
class IManagedEntity : public EntitiesManager::IEntity
{
  friend class EntitiesManager;
public:
  virtual ~IManagedEntity(){}
  IManagedEntity() = delete;
  IManagedEntity(IManagedEntity & other) = delete;
  IManagedEntity(int id, const char * name = nullptr) :
      EntitiesManager::IEntity(id, name) {}
};

} // end namespace Engine

#endif // ENTITIES_MANAGER_HPP
