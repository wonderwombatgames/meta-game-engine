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

//
class EntitiesManager
{
public:

  class IEntity
  {
  public:
    virtual ~IEntity()
    {
      this->tearDownComponents();
    }

    int getId()
    {
      return this->_entityId;
    }

    bool suspend();

    bool resume();

    bool isActive()
    {
      return this->_isActive;
    }

    void destroy()
    {
      this->_mustDestroy = true;
    }

    bool mustDestroy()
    {
      return this->_mustDestroy;
    }

    bool isInSystem(ISystem * system)
    {
      return (0 < this->_componentSystems.count(system));
    }

  protected:
    int _entityId;
    bool _isActive;
    bool _mustDestroy;
    string _name;
    set< ISystem * > _componentSystems;

    IEntity(int id, const char * name = nullptr):
        _entityId(id),
        _isActive(true),
        _mustDestroy(false),
        _name(name)
         {};

    // tear down (de register) components from systems
    void tearDownComponents();

    // must be overriden on each entityType
    // to register components into systems
    virtual void setUpComponents() {};

  };

  static EntitiesManager * instance();

  template< class EntityType = IEntity >
  int createEntity(const char * name);

  bool destroyEntity(int id);

  void updateComponents(ISystem * system);

  int refreshEntities();

  int size()
  {
    return this->_size;
  }

protected:
  enum {  MAX_ENTITIES_AMOUNT = 1000  };

  int newId();

  EntitiesManager() : _size(0) {};

  typedef shared_ptr<IManagedEntity> IEntityPtr;
  typedef unordered_map<int, IEntityPtr> Entities;

  int _size;
  Entities _entities;
  deque<int> _activeEntities;
};


// un-nesting entity class
class IManagedEntity : public EntitiesManager::IEntity
{
public:
  virtual ~IManagedEntity(){}
protected:
  IManagedEntity(int id) : EntitiesManager::IEntity(id){}
};


template< class EntityType >
inline int EntitiesManager::createEntity(const char * name)
{
  int id = this->newId();
  EntityType * entity = new EntityType(id, name);
  entity->setupComponents();
  this->_entities[id].reset(entity);
  this->_size = this->refreshEntities();
  return id;
}

} // end namespace Engine

#endif // ENTITIES_MANAGER_HPP
