/**
  *
  */

#include "entity_transformable.hpp"

namespace Engine
{
using namespace std;
using namespace System;


  EntityTransformable::EntityTransformable(EntityID id)
      :EntityBase(id)
  {
  }
  EntityTransformable::~EntityTransformable()
  {
  }
  // can be used to register components into systems from constructor
  void EntityTransformable::setUpComponents()
  {
    SystemsInterface * sys = System::SystemsInterface::getSystem("Transform");
    assert(sys != nullptr);
    addComponent(*sys);
  }

} // end namespace Engine
