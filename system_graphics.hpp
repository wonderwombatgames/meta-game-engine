/**
  * entities manager contains all entities
  * entities are just containers to components
  *
  *
  */

#ifndef GRAPHIC_SYSTEMS_HPP
#define GRAPHIC_SYSTEMS_HPP

#include <cassert>
#include <string>
#include <unordered_map>
#include <memory>

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
    virtual void setResolution(Dimension3 & res) =0;
    virtual const Dimension3 & getResolution() const =0;
    virtual void setViewRect(const BoxBoundXYWH & rect) =0;
    virtual const BoxBoundXYWH & getViewRect() const =0;
    virtual void setTitle(const string & title) =0;
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
    virtual void setResolution(Dimension3 & res) override;
    virtual const Dimension3 & getResolution() const override;
    virtual void setViewRect(const BoxBoundXYWH & rect) override;
    virtual const BoxBoundXYWH & getViewRect() const override;
    virtual void setTitle(const string & title) override;
    virtual const char * getTitle() const override;
    virtual void setFullscreen(bool fs) override;
    virtual const bool isFullscreen() const override;

  protected:

    typedef T _HANDLER;

    //data
    unique_ptr< _HANDLER > _data;
    BoxBoundXYWH _rect;
    Colour _background;
  };

} // end namespace  Graphics

namespace System
{

  class Graphics : public SystemsInterface
  {
  public:
    shared_ptr<GraphicDevice::DisplayInterface> display;

    Graphics();
    Graphics(const char * name);
    Graphics(Graphics & other) = delete;
    virtual ~Graphics();

    void createDisplay(const BoxBoundXYWH & rect, Flags flags = 0);

    template <typename T>
    AssetID loadAssetFromFile(const string & filepath);
    // TODO:
    // AssetID loadAssetFromAtlas(const ImageAtlas & atlas);
    // AssetID loadAssetFromNet(const NetworkResource & netRes)

    bool setEntityAsset(EntityID entityId, int assetId);

  protected:
    virtual void add(const Component::EntityPod & entity, Component::TransformPod * transform) override;
    virtual void del(const Component::EntityPod & entity) override;
    virtual void tick(TimeDimension delta) override;

    unordered_map< EntityID, Component::GraphicPod > _components;
    unordered_map< AssetID, shared_ptr< Component::GraphicInterface > > _assets;
  };

  template< typename T >
  AssetID Graphics::loadAssetFromFile(const string & filepath)
  {
    static AssetID assetCounter = 0;
    assetCounter = newId();
    this->_assets.emplace(assetCounter, make_shared<T>(filepath));

    return assetCounter;
  }


} // end namespace System

} // end namespace Engine

#endif // GRAPHIC_SYSTEMS_HPP
