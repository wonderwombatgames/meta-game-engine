/**
  *
  *
  */

#include "system_interface.hpp"

namespace W2E
{

namespace System
{

// inlined methods
void SystemsInterface::update(TimeDim delta) { this->tick(delta); }

void SystemsInterface::insertEntity(Component::EntityPod& entity) { this->insert(entity); }

void SystemsInterface::removeEntity(const Component::EntityPod& entity) { this->remove(entity); }

ComponentBinderPtr SystemsInterface::bindComponent(ResourceID resourceId)
{
  return this->getComponentBinder(resourceId);
}

bool SystemsInterface::isValid(SystemsInterface& system)
{
  bool retVal = false;
  SystemsInterface::systemRegistrar(retVal, &system, nullptr, VERIFY);
  return retVal;
}

// class methods
SystemsInterface* SystemsInterface::getSystem(const String& name)
{
  bool success = false;
  SystemsInterface* retPtr = nullptr;
  retPtr = SystemsInterface::systemRegistrar(success, nullptr, name.c_str(), VERIFY);
  assert(success);
  return retPtr;
}

SystemsInterface* SystemsInterface::systemRegistrar(bool& retValue,
                                                    SystemsInterface* inSystem,
                                                    const char* name,
                                                    eRegistrar op)
{
  LOCALPERSISTENT_ HashMap< String, SystemsInterface* > ssystems_;
  SystemsInterface* outSystem = nullptr;

  String searchName;
  if(nullptr != name)
  {
    searchName = name;
  }
  else if(nullptr != inSystem)
  {
    searchName = inSystem->getName();
  }

  if(op == REGISTER)
  {
    if(nullptr != inSystem)
    {
      ssystems_[searchName] = inSystem;
      retValue = true;
      return inSystem;
    }
    else
    {
      retValue = false;
      return outSystem;
    }
  }
  else if(op == UNREGISTER || op == VERIFY)
  {
    for(auto s : ssystems_)
    {
      if(s.second->getName() == searchName)
      {
        if(op == UNREGISTER)
        {
          ssystems_.erase(searchName);
        }
        else
        {
          outSystem = ssystems_.at(searchName);
        }
        retValue = true;
        return outSystem;
      }
    }
  }
  retValue = false;
  return outSystem;
}

} // end namespace System

} // end namespace W2E
