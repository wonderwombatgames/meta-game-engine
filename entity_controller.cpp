/**
  *
  */

#include "entity_controller.hpp"
#include "system_interface.hpp"

namespace W2E
{

using namespace System;


  EntityController::EntityController(EntityID id)
      :EntityBase(id)
  {
  }
  EntityController::~EntityController()
  {
  }
  // can be used to register components into systems from constructor
  void EntityController::setUpComponents()
  {
    SystemsInterface * sys = System::SystemsInterface::getSystem("Controller");
    assert(sys != nullptr);
    registerIntoSystem(*sys);
  }

} // end namespace W2E
