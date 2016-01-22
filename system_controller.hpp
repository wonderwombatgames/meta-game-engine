/**
  *
  */

#ifndef SYSTEM_CONTROLLER_HPP
#define SYSTEM_CONTROLLER_HPP

#include "system_interface.hpp"
#include "component_controller.hpp"

namespace W2E
{
using namespace std;

namespace System
{

class Controller : public SystemsInterface
{
public:
  Controller();
  Controller(const char* name);
  Controller(Controller& other) = delete;
  virtual ~Controller();

protected:
  virtual void insert(Component::EntityPod& entity) override;
  virtual void remove(const Component::EntityPod& entity) override;
  virtual void tick(TimeDim delta) override;

  HashMap< EntityID, Component::ControllerPod > _components;
};

} // end namespace System

} // end namespace W2E

#endif // SYSTEM_CONTROLLER_HPP
