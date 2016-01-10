/**
  *
  */

#ifndef ENTITY_TRANSFORMABLE_HPP
#define ENTITY_TRANSFORMABLE_HPP

#include "entity_base.hpp"

namespace Engine
{
using namespace std;
using namespace System;

class EntityTransformable :  public EntityBase
{
public:
  friend class EntitiesManager;
  // ctors and dtor
  EntityTransformable(EntityID id);
  EntityTransformable() = delete;
  EntityTransformable(EntityTransformable & other) = delete;
  virtual ~EntityTransformable();

protected:
  // can be used to register components into systems from constructor
  virtual void setUpComponents() override;

};

} // end namespace Engine

#endif // ENTITY_TRANSFORMABLE_HPP
