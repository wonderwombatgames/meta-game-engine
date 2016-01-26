/**
  * base system interface
  * other systems will inherity from this class
  *
  *
  */

#ifndef SYSTEM_INTERFACE_HPP
#define SYSTEM_INTERFACE_HPP

#include "component_binder.hpp"
#include "entities_manager.hpp"

namespace W2E
{

namespace System
{

class SystemsInterface
{
public:
  SystemsInterface() = delete;
  SystemsInterface(SystemsInterface& other) = delete;
  SystemsInterface(const char* name)
      : name_(name)
  {
    bool succeded = false;
    SystemsInterface::systemRegistrar(succeded, this, name, REGISTER);
    assert(succeded);
  }

  virtual ~SystemsInterface()
  {
    bool succeded = false;
    SystemsInterface::systemRegistrar(succeded, this, name_.c_str(), UNREGISTER);
    assert(succeded);
  }

  // get system name
  const String& getName() { return this->name_; }
  // perform one step in the system
  void update(TimeDim delta);

  // public class's methods
  // get system by name
  CLASSMETHOD_ SystemsInterface* getSystem(const String& name);
  // check if the pointer actually points to a system
  CLASSMETHOD_ bool isValid(SystemsInterface& system);

  // entity related methods
  void insertEntity(Component::EntityPod& entity);
  void removeEntity(const Component::EntityPod& entity);
  ComponentBinderPtr bindComponent(ResourceID resourceId);

protected:
  enum eRegistrar
  {
    REGISTER,
    UNREGISTER,
    VERIFY,
  };

  // must be overriden in each system (impl. NVI)
  virtual void tick(TimeDim delta) {}
  virtual void insert(Component::EntityPod& entity) {}
  virtual void remove(const Component::EntityPod& entity) {}
  virtual ComponentBinderPtr getComponentBinder(ResourceID resourceId)
  {
    return ComponentBinderPtr(nullptr);
  }

  // class methods
  CLASSMETHOD_ SystemsInterface* systemRegistrar(bool& retValue,
                                                 SystemsInterface* system,
                                                 const char* name = nullptr,
                                                 eRegistrar op = VERIFY);

  // data
  String name_;
};

} // end namespace System

} // end namespace W2E

#endif // SYSTEM_INTERFACE_HPP
