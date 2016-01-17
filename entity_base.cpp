/**
  *
  */


#include "system_interface.hpp"
#include "entity_base.hpp"

namespace W2E
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

ErrorCode EntityBase::suspend()
{
  if (this->_entityData.isActive)
  {
    this->_entityData.isActive = false;
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

ErrorCode EntityBase::resume()
{
  if (!this->_entityData.isActive)
  {
    this->_entityData.isActive = true;
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
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
  this->_componentSystems.clear();
}

// add one more component to the entity
EntityID EntityBase::registerIntoSystem(SystemsInterface & system)
{
  if (SystemsInterface::isValid(system))
  {
    if(this->_componentSystems.count(&system) == 0)
    {
      this->_componentSystems.insert(&system);
      system.insertEntity(this->_entityData);
    }
    return this->_entityData.entityId;
  }
  return InvalidID;
}


ErrorCode EntityBase::deregisterFromSystem(System::SystemsInterface & system)
{
  if (SystemsInterface::isValid(system))
  {
    if(this->_componentSystems.count(&system) > 0)
    {
      system.removeEntity(this->_entityData);
      this->_componentSystems.erase(&system);
      return NO_ERROR;
    }
  }
  return UNKNOWN_ERROR;
}

} // end namespace W2E
