/**
  * base system class
  * other systems will inherity from this class
  *
  *
  */

#ifndef BASE_SYSTEMS_HPP
#define BASE_SYSTEMS_HPP

#include <map>
#include "component_data.hpp"

namespace Engine
{

using namespace std;

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

template< class Update >
class BaseSystem
{
public:
  virtual ~BaseSystem(){}
  virtual void addEntity(int entityId){};
  virtual void delEntity(int entityId){};
  virtual void runEntity(int entityId){};
protected:
  Update updateFunction;
};

} // end namespace Engine

#endif // BASE_SYSTEMS_HPP
