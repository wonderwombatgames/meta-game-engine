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
  virtual ~ISystem(){}
  void addEntity(int entityId){};
  void delEntity(int entityId){};
  void runEntity(IManagedEntity * entity)
  {
    this->run(entity);
  }
  static bool isValid(ISystem * system);
 protected:
  // must be overriden in each system (impl. NVI)
  virtual void run(IManagedEntity * entity){};
};

typedef shared_ptr<ISystem> ISystemPtr;

inline bool ISystem::isValid(ISystem * system)
{
  static set< ISystem * > s_systems;
  return (0 < s_systems.count(system));
}

} // end namespace Engine

#endif // SYSTEM_INTERFACE_HPP
