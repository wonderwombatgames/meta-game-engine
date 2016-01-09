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
class IManagedEntity;
typedef unsigned long long int FrameCount;
typedef float TimeDimension;

namespace System
{

  class SystemsInterface
  {
  public:
    SystemsInterface() = delete;
    SystemsInterface(SystemsInterface & other) = delete;
    SystemsInterface(const char * name): _name(name), _frames(0)
    {
      assert(SystemsInterface::systemRegistrar(this, name, REGISTER));
    }
    virtual ~SystemsInterface()
    {
      assert(SystemsInterface::systemRegistrar(this, _name.c_str(), UNREGISTER));
    }

    // get system name
    const string & getName() { return this->_name;  }
    // check if the pointer actually points to a system
    static bool isValid(SystemsInterface & system);
    // perform one step in the system
    FrameCount update(TimeDimension delta)
    {
      this->tick(delta);
      return ++(this->_frames);
    }

    // entity related methods
    void addEntity(const Component::EntityPod & entity, Component::TransformPod transform);
    void delEntity(const Component::EntityPod & entity);

   protected:
    enum eRegistrar
    {
      REGISTER,
      UNREGISTER,
      VERIFY,
    };

    // must be overriden in each system (impl. NVI)
    virtual void tick(TimeDimension delta){}
    virtual void add(const Component::EntityPod & entity, Component::TransformPod transform){}
    virtual void del(const Component::EntityPod & entity){}
    static bool systemRegistrar(SystemsInterface * system, const char * name = nullptr, eRegistrar op = VERIFY);

    string _name;
    FrameCount _frames;
  };

  inline void SystemsInterface::addEntity(const Component::EntityPod & entity, const Component::TransformPod transform)
  {
    this->add(entity, transform);
  }

  inline void SystemsInterface::delEntity(const Component::EntityPod & entity)
  {
    this->del(entity);
  }

  inline bool SystemsInterface::isValid(SystemsInterface & system)
  {
    return SystemsInterface::systemRegistrar(&system, nullptr, VERIFY);
  }

  inline bool SystemsInterface::systemRegistrar(
      SystemsInterface * system,
      const char * name,
      eRegistrar op)
  {
    static map< string, SystemsInterface * > s_systems;

    string searchName;
    if (nullptr != name)
    {
      searchName = name;
    }
    else if (nullptr != system)
    {
      searchName = system->getName();
    }

    if(op == REGISTER)
    {
      if (nullptr != system)
      {
        s_systems[searchName] = system;
        return true;
      }
      else
      {
        return false;
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
            s_systems.erase(string(name));
          }
          return true;
        }
      }
    }
    return false;
  }

} // end namespace System

} // end namespace Engine

#endif // SYSTEM_INTERFACE_HPP
