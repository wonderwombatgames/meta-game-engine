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

//
// IEntity Methods
//

EntitiesManager::IEntity::IEntity(EntityID id, const char * name):
  _destroy(false),
  _name(name)
{
  // defaults
  this->_entityData.entityId = id;

  // whether or not this entity is active
  this->_entityData.isActive = true;

  // kind of space 2D/3D
  this->_transformData.kind = SPACE_2D;

  // position
  // absolute in pixels (float values)
  // can contain z-order
  this->_transformData.position = {0.0f, 0.0f, 0.0f};

  // rotation
  // values between 0.0 - 1.0  (= 0 - 360)
  this->_transformData.rotation = {0.0f, 0.0f, 0.0f};

  // scales
  // <1.0 : smaller | > 1.0 : larger
  // <0.0 : mirror
  this->_transformData.scale = {1.0f, 1.0f, 1.0f};

}

EntitiesManager::IEntity::~IEntity()
{
  this->tearDownComponents();
}

bool EntitiesManager::IEntity::suspend()
{
  if (this->_entityData.isActive)
  {
    this->_entityData.isActive = false;
    return true;
  }
  return false;
}

bool EntitiesManager::IEntity::resume()
{
  if (!this->_entityData.isActive)
  {
    this->_entityData.isActive = true;
    return true;
  }
  return false;
}

// tear down (de register) components from systems
void EntitiesManager::IEntity::tearDownComponents()
{
  for(auto system : this->_componentSystems)
  {
    if (ISystem::isValid(*system))
    {
      system->delEntity(this->_entityData);
    }
  }
}

// add one more component to the entity
void EntitiesManager::IEntity::addComponent(ISystem & system)
{
  if (ISystem::isValid(system))
  {
    this->_componentSystems.insert(&system);
    system.addEntity(this->_entityData, this->_transformData);
  }
}

//
// EntitiesManager Methods
//

EntitiesManager * EntitiesManager::instance()
{
  static EntitiesManager * s_instance = nullptr;
  if (nullptr == s_instance)
  {
    s_instance = new EntitiesManager();
  }
  return s_instance;
}

bool EntitiesManager::addComponent(EntityID entitityId, ISystem & system)
{
  auto entity = this->_entities.find(entitityId);
  if(entity != this->_entities.end())
  {
    entity->second->addComponent(system);
    return true;
  }
  return false;
}

bool EntitiesManager::destroyEntity(EntityID entityId)
{
  auto entity = this->_entities.find(entityId);
  if (entity != this->_entities.end())
  {
    entity->second->destroy(true);
    return true;
  }
  return false;
}

bool EntitiesManager::suspendEntity(EntityID entityId)
{
  auto entity = this->_entities.find(entityId);
  if (entity != this->_entities.end())
  {
    return entity->second->suspend();
  }
  return false;
}

bool EntitiesManager::resumeEntity(EntityID entityId)
{
  auto entity = this->_entities.find(entityId);
  if (entity != this->_entities.end())
  {
    return entity->second->resume();
  }
  return false;
}


bool EntitiesManager::isEntityActive(EntityID entityId)
{
  auto entity = this->_entities.find(entityId);
  if (entity != this->_entities.end())
  {
    return entity->second->isActive();
  }
  return false;
}

const string * EntitiesManager::lookUpentityName(EntityID entityId)
{

  auto entity = this->_entities.find(entityId);
  if (entity != this->_entities.end())
  {
    return &(entity->second->getName());
  }
  return nullptr;
}

EntityID EntitiesManager::lookUpEntityId(const string& name)
{
  auto entity = this->_lookUp.find(name);
  if (entity != this->_lookUp.end())
  {
    return entity->second;
  }
  return InvalidID;
}

int EntitiesManager::refreshEntities()
{
  // find all entities marked to be destroied
  deque<EntityID> toDestroy;
  for(auto entity : this->_entities)
  {
    if(entity.second->destroy())
    {
      toDestroy.push_back(entity.second->_entityData.entityId);
    }
  }
  // destroy the marked entities
  for(auto id : toDestroy)
  {
    this->_entities.erase(id);
  }
  this->_count = this->_entities.size();
  // return number of entities after clean up
  return this->_count;
}

EntityID EntitiesManager::newId()
{
  const EntityID maxEntities = EntitiesManager::MAX_ENTITIES_AMOUNT;
  // use current time as seed for random generator
  srand(time(0));
  EntityID randomId = 1 + ((rand() * (maxEntities + 1)) % maxEntities);
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
