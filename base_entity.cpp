/**
  *
  */


#include "system_interface.hpp"
#include "base_entity.hpp"

namespace Engine
{
using namespace std;
using namespace System;

//
// BaseEntity Methods
//

BaseEntity::BaseEntity(EntityID id, const char * name)
  :_destroy(false)
  ,_name(name)
  ,_transformData(nullptr)
{
  // defaults
  this->_entityData.entityId = id;

  // whether or not this entity is active
  this->_entityData.isActive = true;

  // this is the base entity so we set it to 1
  this->_entityData.typeId = 1;
}

BaseEntity::~BaseEntity()
{
  this->tearDownComponents();
}

bool BaseEntity::suspend()
{
  if (this->_entityData.isActive)
  {
    this->_entityData.isActive = false;
    return true;
  }
  return false;
}

bool BaseEntity::resume()
{
  if (!this->_entityData.isActive)
  {
    this->_entityData.isActive = true;
    return true;
  }
  return false;
}

// tear down (de register) components from systems
void BaseEntity::tearDownComponents()
{
  for(auto system : this->_componentSystems)
  {
    if (SystemsInterface::isValid(*system))
    {
      system->delEntity(this->_entityData);
    }
  }
}

// add one more component to the entity
void BaseEntity::addComponent(SystemsInterface & system)
{
  if (SystemsInterface::isValid(system))
  {
    this->_componentSystems.insert(&system);
    system.addEntity(this->_entityData, this->_transformData);
  }
}


} // end namespace Engine
