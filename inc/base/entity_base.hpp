/**
  * entities manager contains all entities
  * entities are just containers for components
  *
  *
  */

#ifndef ENTITY_BASE_HPP
#define ENTITY_BASE_HPP

#include "components/binder.hpp"
#include "utils/types.hpp"
namespace W2E
{

using namespace Utils;

namespace Component
{

// forward declaration
struct TransformPod;

struct EntityPod
{
  // entity unique identifier
  EntityID entityId;

  // define to what class entity belongs
  TypeID typeId;

  // whether or not this entity is active
  bool isActive;

  // pointer to transform pod
  TransformPod* transform;
};

} // namespace Component

using namespace Utils;

class EntityBase : public EntityRegistrarInterface
{
public:
  friend class EntitiesManager;
  // friend class SystemsInterface;

  // ctors and dtor
  explicit EntityBase(EntityID id);
  EntityBase() = delete;
  EntityBase(EntityBase& other) = delete;
  virtual ~EntityBase();

  ErrorCode suspend();
  ErrorCode resume();
  ErrorCode destroy(bool mustDestroy = true);
  bool willDestroy();
  bool isActive() const;
  // verify if entity has component
  bool containedInSystem(System::SystemsInterface& system);

protected:
  // add one more component to the entity
  virtual EntityID registerIntoSystem_(System::SystemsInterface& system) override;

  // can be used to register components into systems from constructor
  virtual void setUpComponents(){};

  // tear down (de register) components from systems
  void tearDownComponents();

  // set holds the systems to wich this entity was added
  Set< System::SystemsInterface* > componentSystems_;

  // this POD is to be used by the component systems
  Component::EntityPod entityData_;

  // data
  bool destroy_;
};

inline bool EntityBase::isActive() const { return this->entityData_.isActive; }

inline ErrorCode EntityBase::destroy(bool mustDestroy)
{
  this->destroy_ = mustDestroy;
  return (this->destroy_) ? NO_ERROR : UNKNOWN_ERROR;
}

inline bool EntityBase::willDestroy() { return this->destroy_; }

inline bool EntityBase::containedInSystem(System::SystemsInterface& system)
{
  return (0 < this->componentSystems_.count(&system));
}

} // end namespace W2E

#endif // ENTITY_BASE_HPP
