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

  template < typename T >
  class Display
  {
  public:
    ~Display(){};
    Display(const BoxBoundXYWH & rect, Flags flags = 0);
    Display() = delete;
    Display(Display & other) = delete;

    // rendering
    void render();
    void clear();
    void clear(const Colour & c);

    // reseting the viewport
    void setColour(const Colour & c);
    const Colour & getColour() const;
    void setResolution(Dimension3 & res);
    const Dimension3 & getResolution() const;
    void setViewRect(const BoxBoundXYWH & rect);
    const BoxBoundXYWH & getViewRect() const;
    void setTitle(const string & title);
    const char * getTitle() const;
    void setFullscreen(bool fs);
    const bool isFullscreen() const;

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
    Graphics(const char * name);
    Graphics() = delete;
    Graphics(Graphics & other) = delete;
    virtual ~Graphics();

    template <typename T>
    int loadAssetFromFile(const string & filepath);
    // TODO:
    // int loadAssetFromAtlas(const ImageAtlas & atlas);
    // int loadAssetFromNet(const NetworkResource & netRes)

    bool setEntityAsset(Component::EntityPod entity, int assetId);

  protected:
    virtual void add(const Component::EntityPod & entity, Component::TransformPod transform) override;
    virtual void del(const Component::EntityPod & entity) override;
    virtual void tick(TimeDimension delta) override {};

    unordered_map< EntityID, Component::GraphicPod > _components;
    unordered_map< AssetID, shared_ptr< Component::GraphicInterface > > _assets;
  };


} // end namespace System

} // end namespace Engine

#endif // GRAPHIC_SYSTEMS_HPP
