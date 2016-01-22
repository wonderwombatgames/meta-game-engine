/**
  * base system interface
  * other systems will inherity from this class
  *
  *
  */

#ifndef SYSTEM_INTERFACE_HPP
#define SYSTEM_INTERFACE_HPP

#include <cassert>

#include "component_entity.hpp"
#include "component_transform.hpp"
#include "entities_manager.hpp"

namespace W2E
{
using namespace std;

namespace System
{

class SystemsInterface;

class ComponentBinder
{
public:
  // virtual dtor
  virtual ~ComponentBinder() {}

  // bind this resource to entity
  virtual ErrorCode toEntity(EntityRegistrar* er) { return UNKNOWN_ERROR; }

protected:
  EntityID registerIntoSystem_(SystemsInterface* sys, EntityRegistrar* er)
  {
    if(sys != nullptr && er != nullptr)
    {
      return er->registerIntoSystem(*sys);
    }
    return InvalidID;
  }
};

using ComponentBinderPtr = SharedPtr< ComponentBinder >;

class SystemsInterface
{
public:
  SystemsInterface() = delete;
  SystemsInterface(SystemsInterface& other) = delete;
  SystemsInterface(const char* name)
      : name_(name)
      , frames_(0)
  {
    bool retVal = false;
    SystemsInterface::systemRegistrar(retVal, this, name, REGISTER);
    assert(retVal);
  }

  virtual ~SystemsInterface()
  {
    bool retVal = false;
    SystemsInterface::systemRegistrar(retVal, this, name_.c_str(), UNREGISTER);
    assert(retVal);
  }

  // get system name
  const String& getName() { return this->name_; }
  // perform one step in the system
  FrameCount update(TimeDim delta);

  // public class methods
  // get system by name
  CLASSMETHOD_ SystemsInterface* getSystem(const String& name);
  // check if the pointer actually points to a system
  CLASSMETHOD_ bool isValid(SystemsInterface& system);

  // entity related methods
  void insertEntity(Component::EntityPod& entity);
  void removeEntity(const Component::EntityPod& entity);
  ComponentBinderPtr bindComponent(ResourceID resourceId);

protected:
  enum eRegistrar
  {
    REGISTER,
    UNREGISTER,
    VERIFY,
  };

  // must be overriden in each system (impl. NVI)
  virtual void tick(TimeDim delta) {}
  virtual void insert(Component::EntityPod& entity) {}
  virtual void remove(const Component::EntityPod& entity) {}
  virtual ComponentBinderPtr getComponentBinder(ResourceID resourceId)
  {
    return make_shared< ComponentBinder >();
  }

  // class methods
  CLASSMETHOD_ SystemsInterface* systemRegistrar(bool& retValue,
                                                 SystemsInterface* system,
                                                 const char* name = nullptr,
                                                 eRegistrar op = VERIFY);

  // data
  String name_;
  FrameCount frames_;
};

// inlined methods
inline FrameCount SystemsInterface::update(TimeDim delta)
{
  this->tick(delta);
  return ++(this->frames_);
}

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

#endif // SYSTEM_INTERFACE_HPP
