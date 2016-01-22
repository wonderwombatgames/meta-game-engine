/**
  *
  */

#include "entity_transformable.hpp"
#include "system_interface.hpp"

namespace W2E
{

using namespace System;

EntityTransformable::EntityTransformable(EntityID id)
    : EntityBase(id)
{
}
EntityTransformable::~EntityTransformable() {}
// can be used to register components into systems from constructor
void EntityTransformable::setUpComponents()
{
  SystemsInterface* sys = System::SystemsInterface::getSystem("Transform");
  assert(sys != nullptr);
  registerIntoSystem(*sys);
}

} // end namespace W2E
