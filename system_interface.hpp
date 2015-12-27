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


namespace Engine
{
using namespace std;

#if 0
  namespace
  {
    struct EmptyType {};
  }

  template< class Component, class AuxComponent = EmptyType >
  class UpdateFunction
  {
  public:
    typedef Component ComponentDataType;
    typedef AuxComponent AuxComponentDataType;

    void operator()(Component * data, AuxComponent * aux = nullptr)
    {
      // use Component data
    }
  };
#endif

// forward declare entity type interface
class IManagedEntity;
typedef unsigned long long int FrameCount;
//
class ISystem
{
public:
  virtual ~ISystem(){}
  ISystem() = delete;
  ISystem(ISystem & other) = delete;
  ISystem(const char * name = "empty"): _name(name), _frames(0)
  {
    assert(ISystem::systemRegistrar(this, name));
  }

  const string & getName()
  {
    return this->_name;
  }

  FrameCount getFrame()
  {
    return this->_frames;
  }

  void addEntity(int entityId){};
  void delEntity(int entityId){};

  void runEntity(IManagedEntity * entity)
  {
    this->run(entity);
    ++(this->_frames);
  }

  // check if the pointer actually points to a system
  static bool isValid(ISystem * system);

 protected:
  // must be overriden in each system (impl. NVI)
  virtual void run(IManagedEntity * entity){};

  static bool systemRegistrar(ISystem * system, const char * name = nullptr);

  string _name;

  FrameCount _frames;
};

inline bool ISystem::isValid(ISystem * system)
{
  return ISystem::systemRegistrar(system, nullptr);
}

inline bool ISystem::systemRegistrar(ISystem * system, const char * name)
{
  static map< string, ISystem * > s_systems;

  if (nullptr == name)
  // in this case we are checking if the system is registered.
  {
    string searchName(system->getName());
    for (auto s: s_systems)
    {
      if(s.second->getName() == searchName)
      {
        return true;
      }
    }
  }
  else if (nullptr != system)
  // here we are registering a new system.
  {

    s_systems[string(name)] = system;
    return true;
  }
  // otherwise fail!
  return false;
}

} // end namespace Engine

#endif // SYSTEM_INTERFACE_HPP
