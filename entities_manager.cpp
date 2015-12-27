/**
  *
  */

#include <cstdlib>
#include <ctime>
#include <cassert>
#include "system_interface.hpp"
#include "entities_manager.hpp"

namespace Engine
{
using namespace std;

bool EntitiesManager::IEntity::suspend()
{
  if (this->_isActive)
  {
    this->_isActive = false;
    return true;
  }
  return false;
}

bool EntitiesManager::IEntity::resume()
{
  if (!this->_isActive)
  {
    this->_isActive = true;
    return true;
  }
  return false;
}

// tear down (de register) components from systems
void EntitiesManager::IEntity::tearDownComponents()
{
  for(auto system : this->_componentSystems)
  {
    if (ISystem::isValid(system))
    {
      system->delEntity(this->getId());
    }
  }
}

EntitiesManager * EntitiesManager::instance()
{
  static EntitiesManager * _instance = nullptr;
  if (nullptr == _instance)
  {
    _instance = new EntitiesManager();
  }
  return _instance;
}

bool EntitiesManager::destroyEntity(int id)
{
  auto entity = this->_entities.find(id);
  if (entity->second)
  {
    entity->second->destroy();
    return true;
  }
  return false;
}

void EntitiesManager::updateComponents(ISystem * system)
{
  if (ISystem::isValid(system))
  {
    for(auto entityId : this->_activeEntities)
    {
      auto entity = this->_entities[entityId];
      if(entity->isInSystem(system))
      {
        system->runEntity(entity.get());
      }
    }
  }
  this->_size = this->refreshEntities();
}

int EntitiesManager::refreshEntities()
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

int EntitiesManager::newId()
{
  const int maxEntities = EntitiesManager::MAX_ENTITIES_AMOUNT;
  // use current time as seed for random generator
  srand(time(0));
  int randomId = (rand() * (maxEntities + 1)) % maxEntities;
  int attempts = 0;
  // check that the id was not used yet
  while (this->_entities.count(randomId)>0)
  {
    randomId = (rand() * (maxEntities + 1)) % maxEntities;
    ++attempts;
    // this ensures we do not allocate more entities than the MAX
    assert( attempts < maxEntities);
  }
  return randomId;
}

} // end namespace Engine
