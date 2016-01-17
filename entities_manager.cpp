/**
  *
  */

#include "system_interface.hpp"
#include "entities_manager.hpp"

namespace W2E
{
using namespace std;
using namespace System;

//
// EntitiesManager Methods
//

EntitiesManager * EntitiesManager::instance()
{
  LOCAL_PERSISTENT EntitiesManager * s_instance = nullptr;
  if (nullptr == s_instance)
  {
    s_instance = new EntitiesManager();
  }
  return s_instance;
}

// const String * EntitiesManager::lookUpEntityName(EntityID entityId)
// {
//
//   auto entity = this->_entities.find(entityId);
//   if (entity != this->_entities.end())
//   {
//     return &(entity->second->getName());
//   }
//   return nullptr;
// }

EntityBase & EntitiesManager::entity(EntityID entityId)
{
  auto entity = this->_entities.find(entityId);
  if (entity != this->_entities.end())
  {
    return *(entity->second.get());
  }
  assert(false);
}


EntityID EntitiesManager::lookUpEntityId(const String& name)
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
  List<EntityID> toDestroy;
  for(auto entity : this->_entities)
  {
    if(entity.second->willDestroy())
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


} // end namespace W2E
