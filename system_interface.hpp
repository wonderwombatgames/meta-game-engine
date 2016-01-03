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

#include "entity_component.hpp"

namespace Engine
{
using namespace std;

// forward declare entity type interface
class IManagedEntity;
typedef unsigned long long int FrameCount;
typedef float TimeDimension;

//
class ISystem
{
public:
  ISystem() = delete;
  ISystem(ISystem & other) = delete;
  ISystem(const char * name): _name(name), _frames(0)
  {
    assert(ISystem::systemRegistrar(this, name, REGISTER));
  }
  virtual ~ISystem()
  {
    assert(ISystem::systemRegistrar(this, _name.c_str(), UNREGISTER));
  }

  // get system name
  const string & getName() { return this->_name;  }
  // check if the pointer actually points to a system
  static bool isValid(ISystem & system);
  // perform one step in the system
  FrameCount update(TimeDimension delta)
  {
    this->tick(delta);
    return ++(this->_frames);
  }

  // entity related methods
  void addEntity(EntityComponent & entity){};
  void delEntity(EntityComponent & entity){};

 protected:
  enum eRegistrar
  {
    REGISTER,
    UNREGISTER,
    VERIFY,
  };

  // must be overriden in each system (impl. NVI)
  virtual void tick(TimeDimension delta){};
  static bool systemRegistrar(ISystem * system, const char * name = nullptr, eRegistrar op = VERIFY);

  string _name;
  FrameCount _frames;
};

inline bool ISystem::isValid(ISystem & system)
{
  return ISystem::systemRegistrar(&system, nullptr, VERIFY);
}

inline bool ISystem::systemRegistrar(
    ISystem * system,
    const char * name,
    eRegistrar op)
{
  static map< string, ISystem * > s_systems;

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

} // end namespace Engine

#endif // SYSTEM_INTERFACE_HPP
