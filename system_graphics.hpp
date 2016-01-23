/**
  *
  *
  */

#ifndef GRAPHIC_SYSTEMS_HPP
#define GRAPHIC_SYSTEMS_HPP

#include <cassert>
#include <memory>

// #include "utils_colour.hpp"
#include "system_interface.hpp"
#include "component_transform.hpp"
#include "component_graphic.hpp"
#include "display_interface.hpp"

namespace W2E
{

namespace System
{

using DisplayHandler = SharedPtr< DisplayInterface >;
using ComponentsHashMap = HashMap< EntityID, Component::GraphicPod >;
using GraphicResource = SharedPtr< Component::GraphicInterface >;
using ResourcesHashMap = HashMap< ResourceID, GraphicResource >;

// class GraphicComponentBinder : public ComponentBinder
// {
// public:
//   // virtual dtor
//   virtual ~GraphicComponentBinder() {}
//
//   // ctor
//   GraphicComponentBinder(SystemsInterface* system,
//                          Component::GraphicInterface* resource,
//                          ComponentsHashMap* components);
//
//   // bind this resource to entity
//   virtual ErrorCode toEntity(EntityRegistrar* sp) override;
//
// private:
//   SystemsInterface* system_;
//   Component::GraphicInterface* resource_;
//   ComponentsHashMap* components_;
// };

class Graphics : public SystemsInterface
{
public:
  DisplayHandler display;

  Graphics();
  Graphics(const char* name);
  Graphics(Graphics& other) = delete;
  virtual ~Graphics();

  DisplayHandler createDisplay(const BoxBoundXYWH& rect, Flags flags = 0);

  template < typename T >
  ResourceID loadResourceFromFile(const String& filepath);
  // TODO:
  // ResourceID loadResourceFromAtlas(const ImageAtlas & atlas);
  // ResourceID loadResourceFromNet(const NetworkResource & netRes)

  // camera set
  ErrorCode setCameraTransform(Component::TransformPod& transformData);

protected:
  virtual void insert(Component::EntityPod& entity) override;
  virtual void remove(const Component::EntityPod& entity) override;
  virtual void tick(TimeDim delta) override;
  virtual ComponentBinderPtr getComponentBinder(ResourceID resourceId) override;

  Component::TransformPod camera_;
  ComponentsHashMap components_;
  ResourcesHashMap resources_;
};

template < typename T >
ResourceID Graphics::loadResourceFromFile(const String& filepath)
{
  ResourceID resCounter = seqId();
  this->resources_.emplace(resCounter, make_shared< T >(filepath));

  return resCounter;
}

} // end namespace System

} // end namespace W2E

#endif // GRAPHIC_SYSTEMS_HPP
