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
inline void SystemsInterface::update(TimeDim delta) { this->tick(delta); }

inline void SystemsInterface::insertEntity(Component::EntityPod& entity) { this->insert(entity); }

inline void SystemsInterface::removeEntity(const Component::EntityPod& entity)
{
  this->remove(entity);
}

inline ComponentBinderPtr SystemsInterface::bindComponent(ResourceID resourceId)
{
  return this->getComponentBinder(resourceId);
}

inline bool SystemsInterface::isValid(SystemsInterface& system)
{
  bool retVal = false;
  SystemsInterface::systemRegistrar(retVal, &system, nullptr, VERIFY);
  return retVal;
}

// class methods
inline SystemsInterface* SystemsInterface::getSystem(const String& name)
{
  bool success = false;
  SystemsInterface* retPtr = nullptr;
  retPtr = SystemsInterface::systemRegistrar(success, nullptr, name.c_str(), VERIFY);
  assert(success);
  return retPtr;
}

inline SystemsInterface* SystemsInterface::systemRegistrar(bool& retValue,
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
