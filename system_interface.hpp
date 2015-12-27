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

// namespace
// {
//   struct EmptyType {};
// }
//
// template< class Component, class AuxComponent = EmptyType >
// class UpdateFunction
// {
// public:
//   typedef Component ComponentDataType;
//   typedef AuxComponent AuxComponentDataType;
//
//   void operator()(Component * data, AuxComponent * aux = nullptr)
//   {
//     // use Component data
//   }
// };

// template< class Update >
class ISystem
{
public:
  static bool isValid(ISystem * system)
  {
    return (0 < ISystem::s_systems.count(system));
  }

  virtual ~ISystem(){}
  virtual void addEntity(int entityId){};
  virtual void delEntity(int entityId){};
  virtual void runEntity(int entityId){};
 protected:
   static set< ISystem * > s_systems;
//   Update updateFunction;
};

typedef shared_ptr<ISystem> ISystemPtr;
typedef weak_ptr<ISystem> ISystemWPtr;

} // end namespace Engine

#endif // SYSTEM_INTERFACE_HPP
