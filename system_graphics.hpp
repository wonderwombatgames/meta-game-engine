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

namespace Engine
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


  template < typename T >
  class Display : public DisplayInterface
  {
  public:
    Display(const BoxBoundXYWH & rect, Flags flags = 0);
    virtual ~Display(){};
    Display() = delete;
    Display(Display & other) = delete;

    // rendering
    virtual void render() override;
    virtual void clear() override;
    virtual void clear(const Colour & c) override;

    // reseting the viewport
    virtual void setColour(const Colour & c) override;
    virtual const Colour & getColour() const override;
    virtual void setResolution(Dimension2 & res) override;
    virtual const Dimension2 & getResolution() const override;
    virtual void setViewRect(const BoxBoundXYWH & rect) override;
    virtual const BoxBoundXYWH & getViewRect() const override;
    virtual void setTitle(const String & title) override;
    virtual const char * getTitle() const override;
    virtual void setFullscreen(bool fs) override;
    virtual const bool isFullscreen() const override;

  protected:

    using _HANDLER = T;

    //data
    UniquePtr< _HANDLER > _data;
    BoxBoundXYWH _rect;
    Colour _background;
  };

} // end namespace  Graphics

namespace System
{

  using DisplayHandler = SharedPtr< GraphicDevice::DisplayInterface >;

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
    AssetID loadAssetFromFile(const String & filepath);
    // TODO:
    // AssetID loadAssetFromAtlas(const ImageAtlas & atlas);
    // AssetID loadAssetFromNet(const NetworkResource & netRes)

    bool setEntityAsset(EntityID entityId, int assetId);

  protected:
    virtual void add(const Component::EntityPod & entity, Component::TransformPod * transform) override;
    virtual void del(const Component::EntityPod & entity) override;
    virtual void tick(TimeDim delta) override;

    HashMap< EntityID, Component::GraphicPod > _components;
    HashMap< AssetID, SharedPtr< Component::GraphicInterface > > _assets;
  };

  template< typename T >
  AssetID Graphics::loadAssetFromFile(const String & filepath)
  {
    static AssetID assetCounter = 0;
    assetCounter = seqId();
    this->_assets.emplace(assetCounter, make_shared<T>(filepath));

    return assetCounter;
  }


} // end namespace System

} // end namespace Engine

#endif // GRAPHIC_SYSTEMS_HPP
