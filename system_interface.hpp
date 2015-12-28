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

// forward declare entity type interface
class IManagedEntity;
typedef unsigned long long int FrameCount;

//
class ISystem
{
public:
  virtual ~ISystem()
  {
    assert(ISystem::systemRegistrar(this, _name.c_str(), UNREGISTER));
  }
  ISystem() = delete;
  ISystem(ISystem & other) = delete;
  ISystem(const char * name): _name(name), _frames(0)
  {
    assert(ISystem::systemRegistrar(this, name, REGISTER));
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
  static bool isValid(ISystem & system);

 protected:
   enum eRegistrar
  {
    REGISTER,
    UNREGISTER,
    VERIFY,
  };

   static bool systemRegistrar(
      ISystem * system,
      const char * name = nullptr,
      eRegistrar op = VERIFY);

  // must be overriden in each system (impl. NVI)
  virtual void run(IManagedEntity * entity){};

  string _name;

  FrameCount _frames;
};

inline bool ISystem::isValid(ISystem & system)
{
  return ISystem::systemRegistrar(&system, nullptr);
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
