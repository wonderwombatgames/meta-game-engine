/**
  * base system interface
  * other systems will inherity from this class
  *
  *
  */

#ifndef SYSTEM_INTERFACE_HPP
#define SYSTEM_INTERFACE_HPP

#include <memory>
#include <set>

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

//
class ISystem
{
public:
  static bool isValid(ISystem * system)
  {
    return (0 < ISystem::s_systems.count(system));
  }

  virtual ~ISystem(){}
  void addEntity(int entityId){};
  void delEntity(int entityId){};
  void runEntity(IManagedEntity * entity)
  {
    this->run(entity);
  }
 protected:
  // must be overriden in each system (impl. NVI)
  virtual void run(IManagedEntity * entity){};

  static set< ISystem * > s_systems;
};

typedef shared_ptr<ISystem> ISystemPtr;

} // end namespace Engine

#endif // SYSTEM_INTERFACE_HPP
