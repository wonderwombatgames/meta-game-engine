/**
  *
  */

#ifndef ENTITY_CONTROLLER_HPP
#define ENTITY_CONTROLLER_HPP

#include "entity_base.hpp"

namespace W2E
{

using namespace System;

class EntityController : public EntityBase
{
public:
  friend class EntitiesManager;
  // ctors and dtor
  EntityController(EntityID id);
  EntityController() = delete;
  EntityController(EntityController& other) = delete;
  virtual ~EntityController();

protected:
  // can be used to register components into systems from constructor
  virtual void setUpComponents() override;
};

} // end namespace W2E

#endif // ENTITY_CONTROLLER_HPP
