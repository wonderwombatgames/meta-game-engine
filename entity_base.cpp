/**
  *
  */


#include "system_interface.hpp"
#include "entity_base.hpp"

namespace Engine
{
using namespace std;
using namespace System;

//
// EntityBase Methods
//

EntityBase::EntityBase(EntityID id)
  :_destroy(false)
{
  // defaults
  this->_entityData.entityId = id;

  // whether or not this entity is active
  this->_entityData.isActive = true;

  // this is the base entity so we set it to 1
  this->_entityData.typeId = 1;

  // pointer to transformation component (used by several systems)
  this->_entityData.transform = nullptr;
}

EntityBase::~EntityBase()
{
  this->tearDownComponents();
}

bool EntityBase::suspend()
{
  if (this->_entityData.isActive)
  {
    this->_entityData.isActive = false;
    return true;
  }
  return false;
}

bool EntityBase::resume()
{
  if (!this->_entityData.isActive)
  {
    this->_entityData.isActive = true;
    return true;
  }
  return false;
}

// tear down (de register) components from systems
void EntityBase::tearDownComponents()
{
  for(auto system : this->_componentSystems)
  {
    if (SystemsInterface::isValid(*system))
    {
      system->removeEntity(this->_entityData);
    }
  }
}

// add one more component to the entity
EntityID EntityBase::addComponent(SystemsInterface & system)
{
  if (SystemsInterface::isValid(system))
  {
    this->_componentSystems.insert(&system);
    system.insertEntity(this->_entityData);
    return this->_entityData.entityId;
    }
  return -1;
}


} // end namespace Engine
