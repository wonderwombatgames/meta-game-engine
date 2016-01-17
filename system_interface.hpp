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

namespace Engine
{
using namespace std;

namespace System
{

  class ResourceBinder
  {
  public:
    // virtual dtor
    virtual ~ResourceBinder(){}

    // bind this resource to entity
    bool toEntity(EntityID entityId){ return false; }
  };

  using ResourceBinderPtr = SharedPtr<ResourceBinder>;

  class SystemsInterface
  {
  public:
    SystemsInterface() = delete;
    SystemsInterface(SystemsInterface & other) = delete;
    SystemsInterface(const char * name): _name(name), _frames(0)
    {
      bool retVal = false;
      SystemsInterface::systemRegistrar(retVal, this, name, REGISTER);
      assert(retVal);
    }

    virtual ~SystemsInterface()
    {
      bool retVal = false;
      SystemsInterface::systemRegistrar(retVal, this, _name.c_str(), UNREGISTER);
      assert(retVal);
    }

    // get system name
    const String & getName() { return this->_name;  }
    // check if the pointer actually points to a system
    static bool isValid(SystemsInterface & system);
    // perform one step in the system
    FrameCount update(TimeDim delta);
    // get system by name
    static SystemsInterface * getSystem(const String & name);

    // entity related methods
    void insertEntity(Component::EntityPod & entity);
    void removeEntity(const Component::EntityPod & entity);
    ResourceBinderPtr bindResource(ResourceID resourceId);

   protected:
    enum eRegistrar
    {
      REGISTER,
      UNREGISTER,
      VERIFY,
    };

    // must be overriden in each system (impl. NVI)
    virtual void tick(TimeDim delta){}
    virtual void insert(Component::EntityPod & entity){}
    virtual void remove(const Component::EntityPod & entity){}
    virtual ResourceBinderPtr getResourceBinder(ResourceID resourceId){
      return make_shared<ResourceBinder>();
    }

    // class methods
    static SystemsInterface * systemRegistrar(bool & retValue, SystemsInterface * system, const char * name = nullptr, eRegistrar op = VERIFY);

    // data
    String _name;
    FrameCount _frames;
  };

  // inlined methods
  inline FrameCount SystemsInterface::update(TimeDim delta)
  {
    this->tick(delta);
    return ++(this->_frames);
  }

  inline void SystemsInterface::insertEntity(Component::EntityPod & entity)
  {
    this->insert(entity);
  }

  inline void SystemsInterface::removeEntity(const Component::EntityPod & entity)
  {
    this->remove(entity);
  }

  inline ResourceBinderPtr SystemsInterface::bindResource(ResourceID resourceId)
  {
    return  this->getResourceBinder(resourceId);
  }


  inline bool SystemsInterface::isValid(SystemsInterface & system)
  {
    bool retVal = false;
    SystemsInterface::systemRegistrar(retVal, &system, nullptr, VERIFY);
    return retVal;
  }

  // class methods
  inline SystemsInterface * SystemsInterface::getSystem(const String & name)
  {
    bool success = false;
    SystemsInterface * retPtr = nullptr;
    retPtr = SystemsInterface::systemRegistrar(success, nullptr, name.c_str(), VERIFY);
    assert(success);
    return retPtr;
  }

  inline SystemsInterface * SystemsInterface::systemRegistrar(bool & retValue, SystemsInterface * inSystem, const char * name, eRegistrar op)
  {
    static HashMap< String, SystemsInterface * > s_systems;
    SystemsInterface *  outSystem = nullptr;

    String searchName;
    if (nullptr != name)
    {
      searchName = name;
    }
    else if (nullptr != inSystem)
    {
      searchName = inSystem->getName();
    }

    if(op == REGISTER)
    {
      if (nullptr != inSystem)
      {
        s_systems[searchName] = inSystem;
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
      for (auto s: s_systems)
      {
        if(s.second->getName() == searchName)
        {
          if(op == UNREGISTER)
          {
            s_systems.erase(searchName);
          }
          else
          {
            outSystem = s_systems.at(searchName);
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

} // end namespace Engine

#endif // SYSTEM_INTERFACE_HPP
