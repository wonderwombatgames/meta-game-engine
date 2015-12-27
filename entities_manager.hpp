/**
  * entities manager contains all entities
  * entities are just containers to components
  *
  *
  */

#ifndef ENTITIES_MANAGER_HPP
#define ENTITIES_MANAGER_HPP

#include <unordered_map>
#include <set>
#include <deque>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include "system_interface.hpp"

namespace Engine
{
using namespace std;

//
class EntitiesManager
{
public:

  //
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

    bool suspend()
    {
      if (this->_isActive)
      {
        this->_isActive = false;
        return true;
      }
      return false;
    }

    bool resume()
    {
      if (!this->_isActive)
      {
        this->_isActive = true;
        return true;
      }
      return false;
    }

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
    set< ISystem * > _componentSystems;


    IEntity(int id):
        _entityId(id),
        _isActive(true),
        _mustDestroy(false)
         {};

    // tear down (de register) components from systems
    void tearDownComponents()
    {
      for(auto system : this->_componentSystems)
      {
        if (ISystem::isValid(system))
        {
          system->delEntity(this->getId());
        }
      }
    }

    // must be overriden on each entityType
    // to register components into systems
    virtual void setUpComponents() {};

  };

  static EntitiesManager * instance()
  {
    static EntitiesManager * _instance = nullptr;
    if (nullptr == _instance)
    {
      _instance = new EntitiesManager();
    }
    return _instance;
  }

  template< class EntityType = IEntity >
  int createEntity(const char * name)
  {
    int id = this->newId();
    EntityType * entity = new EntityType(id, name);
    entity->setupComponents();
    this->_entities[id].reset(entity);
    this->_size = this->refreshEntities();
    return id;
  }

  bool destroyEntity(int id)
  {
    auto entity = this->_entities.find(id);
    if (entity->second)
    {
      entity->second->destroy();
      return true;
    }
    return false;
  }

  void updateComponents(ISystem * system)
  {
    if (ISystem::isValid(system))
    {
      for(auto entityId : this->_activeEntities)
      {
        auto entity = this->_entities[entityId];
        if(entity->isInSystem(system))
        {
          system->runEntity(entity->getId());
        }
      }
    }
    this->_size = this->refreshEntities();
  }

  int refreshEntities()
  {
    // find all entities marked to be destroied
    deque<int> toDestroy;
    this->_activeEntities.clear();
    for(auto entity : this->_entities)
    {
      if(entity.second->mustDestroy())
      {
        toDestroy.push_back(entity.second->getId());
      }
      else if(entity.second->isActive())
      {
        this->_activeEntities.push_back(entity.second->getId());
      }
    }
    // destroy the marked entities
    for(auto id : toDestroy)
    {
      this->_entities.erase(id);
    }
    // return number of entities after clean up
    return this->_entities.size();
  }

  int size()
  {
    return this->_size;
  }

protected:
  enum
  {
    MAX_ENTITIES_AMOUNT = 1000
  };
  int newId()
  {
    srand(time(0)); // use current time as seed for random generator
    int randomId = (rand() * (MAX_ENTITIES_AMOUNT + 1)) % MAX_ENTITIES_AMOUNT;
    int attempts = 0;
    // check that the id was not used yet
    while (this->_entities.count(randomId)>0)
    {
      randomId = (rand() * (MAX_ENTITIES_AMOUNT + 1)) % MAX_ENTITIES_AMOUNT;
      ++attempts;
      // this ensures we do not allocate more entities than the MAX
      assert( attempts < MAX_ENTITIES_AMOUNT);
    }
    return randomId;
  }

  EntitiesManager() : _size(0) {};

  typedef shared_ptr<IEntity> IEntityPtr;
  typedef unordered_map<int, IEntityPtr> Entities;

  int _size;
  Entities _entities;
  deque<int> _activeEntities;
};


} // end namespace Engine

#endif // ENTITIES_MANAGER_HPP
