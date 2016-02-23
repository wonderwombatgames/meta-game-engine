/**
  *
  */

#include "system_interface.hpp"
#include "entities_manager.hpp"

namespace W2E
{

using namespace System;

//
// EntitiesManager Methods
//

EntitiesManager* EntitiesManager::instance()
{
  LOCAL_PERSISTENT EntitiesManager* sinstance_ = nullptr;
  if(nullptr == sinstance_)
  {
    sinstance_ = new EntitiesManager();
  }
  return sinstance_;
}

EntityRegistrarInterface* EntitiesManager::registrar(EntityID entityId)
{
  auto entity = this->entities_.find(entityId);
  if(entity != this->entities_.end())
  {
    return entity->second.get();
  }
  assert(false);
}

// const String * EntitiesManager::lookUpEntityName(EntityID entityId)
// {
//
//   auto entity = this->entities_.find(entityId);
//   if (entity != this->entities_.end())
//   {
//     return &(entity->second->getName());
//   }
//   return nullptr;
// }

EntityID EntitiesManager::lookUpEntityId(const String& name)
{
  auto entity = this->lookUp_.find(name);
  if(entity != this->lookUp_.end())
  {
    return entity->second;
  }
  return InvalidID;
}

int EntitiesManager::refreshEntities()
{
  // find all entities marked to be destroied
  List< EntityID > toDestroy;
  for(auto entity : this->entities_)
  {
    if(entity.second->willDestroy())
    {
      toDestroy.push_back(entity.second->entityData_.entityId);
    }
  }
  // destroy the marked entities
  for(auto id : toDestroy)
  {
    this->entities_.erase(id);
  }
  this->count_ = this->entities_.size();
  // return number of entities after clean up
  return this->count_;
}

} // end namespace W2E
