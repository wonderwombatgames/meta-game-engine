/**
  * base system interface
  * other systems will inherity from this class
  *
  *
  */

#ifndef SYSTEM_INTERFACE_HPP
#define SYSTEM_INTERFACE_HPP

#include <memory>
#include <map>
#include <string>
#include <cassert>

#include "component_entity.hpp"
#include "component_transform.hpp"

namespace Engine
{
using namespace std;

// forward declare entity type interface
class EntityBase;
typedef unsigned long long int FrameCount;
typedef float TimeDim;

namespace System
{

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
    const string & getName() { return this->_name;  }
    // check if the pointer actually points to a system
    static bool isValid(SystemsInterface & system);
    // perform one step in the system
    FrameCount update(TimeDim delta);

    // entity related methods
    void addEntity(const Component::EntityPod & entity, Component::TransformPod * transform);
    void delEntity(const Component::EntityPod & entity);

    static SystemsInterface * getSystem(const string & name);

   protected:
    enum eRegistrar
    {
      REGISTER,
      UNREGISTER,
      VERIFY,
    };

    // must be overriden in each system (impl. NVI)
    virtual void tick(TimeDim delta){}
    virtual void add(const Component::EntityPod & entity, Component::TransformPod * transform){}
    virtual void del(const Component::EntityPod & entity){}
    static SystemsInterface * systemRegistrar(bool & retValue, SystemsInterface * system, const char * name = nullptr, eRegistrar op = VERIFY);

    string _name;
    FrameCount _frames;
  };

  inline FrameCount SystemsInterface::update(TimeDim delta)
  {
    this->tick(delta);
    return ++(this->_frames);
  }

  inline void SystemsInterface::addEntity(const Component::EntityPod & entity, Component::TransformPod * transform)
  {
    this->add(entity, transform);
  }

  inline void SystemsInterface::delEntity(const Component::EntityPod & entity)
  {
    this->del(entity);
  }

  inline bool SystemsInterface::isValid(SystemsInterface & system)
  {
    bool retVal = false;
    SystemsInterface::systemRegistrar(retVal, &system, nullptr, VERIFY);
    return retVal;
  }

  inline SystemsInterface * SystemsInterface::getSystem(const string & name)
  {
    bool success = false;
    SystemsInterface * retPtr = nullptr;
    retPtr = SystemsInterface::systemRegistrar(success, nullptr, name.c_str(), VERIFY);
    assert(success);
    return retPtr;
  }

  inline SystemsInterface * SystemsInterface::systemRegistrar(bool & retValue, SystemsInterface * inSystem, const char * name, eRegistrar op)
  {
    static map< string, SystemsInterface * > s_systems;
    SystemsInterface *  outSystem = nullptr;

    string searchName;
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
