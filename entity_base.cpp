/**
  *
  */

#include "system_interface.hpp"
#include "entity_base.hpp"

namespace W2E
{

using namespace System;

//
// EntityBase Methods
//

EntityBase::EntityBase(EntityID id)
    : EntityRegistrarInterface{}
    , componentSystems_{}
    , entityData_{id, 1, true, nullptr}
    , destroy_{false}
{
}

EntityBase::~EntityBase() { this->tearDownComponents(); }

ErrorCode EntityBase::suspend()
{
  if(this->entityData_.isActive)
  {
    this->entityData_.isActive = false;
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

ErrorCode EntityBase::resume()
{
  if(!this->entityData_.isActive)
  {
    this->entityData_.isActive = true;
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

// tear down (de register) components from systems
void EntityBase::tearDownComponents()
{
  for(auto system : this->componentSystems_)
  {
    if(SystemsInterface::isValid(*system))
    {
      system->removeEntity(this->entityData_);
    }
  }
  this->componentSystems_.clear();
}

// add one more component to the entity
EntityID EntityBase::registerIntoSystem_(SystemsInterface& system)
{
  if(SystemsInterface::isValid(system))
  {
    if(this->componentSystems_.count(&system) == 0)
    {
      this->componentSystems_.insert(&system);
      system.insertEntity(this->entityData_);
    }
    return this->entityData_.entityId;
  }
  return InvalidID;
}

} // end namespace W2E
