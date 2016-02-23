/**
  *
  *
  */

#ifndef GRAPHIC_SYSTEMS_HPP
#define GRAPHIC_SYSTEMS_HPP

// #include "utils_colour.hpp"
#include "component_graphic.hpp"
#include "component_transform.hpp"
#include "display_interface.hpp"
#include "system_interface.hpp"

namespace W2E
{

namespace System
{

using DisplayHandler = SharedPtr< DisplayInterface >;
using GraphicComponents = HashMap< EntityID, Component::GraphicPod >;
using GraphicResourcePtr = SharedPtr< Component::GraphicInterface >;
using GraphicResources = HashMap< ResourceID, GraphicResourcePtr >;

class Graphics : public SystemsInterface
{
public:
  DisplayHandler display;

  Graphics();
  Graphics(const char* name);
  Graphics(Graphics& other) = delete;
  virtual ~Graphics();

  // methods specific to the graphic system
  DisplayHandler createDisplay(const BoxBoundXYWH& rect, Flags flags = 0);

  template < typename T >
  ResourceID loadResourceFromFile(const String& filepath);
  // TODO:
  // ResourceID loadResourceFromAtlas(const ImageAtlas & atlas);
  // ResourceID loadResourceFromNet(const NetworkResource & netRes)

  // camera set
  ErrorCode setCameraTransform(Component::TransformPod& transformData);

protected:
  // this methods override the base class to create the specific graphic system behaviour
  virtual void insert(Component::EntityPod& entity) override;
  virtual void remove(const Component::EntityPod& entity) override;
  virtual void tick(TimeDim delta) override;
  virtual ComponentBinderPtr getComponentBinder(ResourceID resourceId) override;

  // data
  Component::TransformPod camera_;
  GraphicComponents components_;
  GraphicResources resources_;
};

template < typename ResourceType >
ResourceID Graphics::loadResourceFromFile(const String& filepath)
{
  ResourceID resCounter = seqId();
  this->resources_.emplace(resCounter, std::make_shared< ResourceType >(filepath));

  return resCounter;
}

} // end namespace System

} // end namespace W2E

#endif // GRAPHIC_SYSTEMS_HPP
