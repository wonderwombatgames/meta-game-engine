/**
  * entities manager contains all entities
  * entities are just containers to components
  *
  *
  */

#ifndef GRAPHIC_SYSTEMS_HPP
#define GRAPHIC_SYSTEMS_HPP

#include <cassert>
#include <memory>

#include "utils_colour.hpp"
#include "system_interface.hpp"
#include "component_graphic.hpp"

namespace W2E
{
using namespace std;

namespace GraphicDevice
{

  class DisplayInterface
  {
  public:
    virtual ~DisplayInterface(){};

    // rendering
    virtual void render() =0;
    virtual void clear() =0;
    virtual void clear(const Colour & c) =0;

    // reseting the viewport
    virtual void setColour(const Colour & c) =0;
    virtual const Colour & getColour() const =0;
    virtual void setResolution(Dimension2 & res) =0;
    virtual const Dimension2 & getResolution() const =0;
    virtual void setViewRect(const BoxBoundXYWH & rect) =0;
    virtual const BoxBoundXYWH & getViewRect() const =0;
    virtual void setTitle(const String & title) =0;
    virtual const char * getTitle() const =0;
    virtual void setFullscreen(bool fs) =0;
    virtual const bool isFullscreen() const =0;
  };

} // end namespace  GraphicDevice

namespace System
{

  using DisplayHandler = SharedPtr< GraphicDevice::DisplayInterface >;
  using ComponentsHashMap = HashMap< EntityID, Component::GraphicPod >;
  using ResourcesHashMap = HashMap< ResourceID, SharedPtr< Component::GraphicInterface > >;


  class GraphicResourceBinder : public ResourceBinder
  {
  public:
    // virtual dtor
    virtual ~GraphicResourceBinder(){}

    // ctor
    GraphicResourceBinder(
      SystemsInterface * system,
      Component::GraphicInterface * resource,
      ComponentsHashMap * components);

    // bind this resource to entity
    ErrorCode toEntity(EntityBase * entity);

  private:
    SystemsInterface * _system;
    Component::GraphicInterface * _resource;
    ComponentsHashMap * _components;
  };


  class Graphics : public SystemsInterface
  {
  public:
    DisplayHandler display;

    Graphics();
    Graphics(const char * name);
    Graphics(Graphics & other) = delete;
    virtual ~Graphics();

    DisplayHandler createDisplay(const BoxBoundXYWH & rect, Flags flags = 0);

    template <typename T>
    ResourceID loadResourceFromFile(const String & filepath);
    // TODO:
    // ResourceID loadResourceFromAtlas(const ImageAtlas & atlas);
    // ResourceID loadResourceFromNet(const NetworkResource & netRes)

    // camera set
    ErrorCode setCameraTransform(Component::TransformPod & transformData);

  protected:
    virtual void insert(Component::EntityPod & entity) override;
    virtual void remove(const Component::EntityPod & entity) override;
    virtual void tick(TimeDim delta) override;
    virtual ResourceBinderPtr getResourceBinder(ResourceID resourceId) override;

    Component::TransformPod _camera;
    ComponentsHashMap _components;
    ResourcesHashMap _resources;
  };

  template< typename T >
  ResourceID Graphics::loadResourceFromFile(const String & filepath)
  {
    ResourceID resCounter = seqId();
    this->_resources.emplace(resCounter, make_shared<T>(filepath));

    return resCounter;
  }


} // end namespace System

} // end namespace W2E

#endif // GRAPHIC_SYSTEMS_HPP
