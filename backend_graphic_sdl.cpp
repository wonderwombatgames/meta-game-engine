/**
  * implement graphic backend functions using SDL
  *
  */

#include <cassert>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "backend_handler_sdl.hpp"
#include "system_graphics.hpp"


namespace
{
using namespace Engine;
using namespace Utils;

  inline void BoxBoundXYWH2SDLRect(const BoxBoundXYWH & box, SDL_Rect & rect)
  {
    rect.x = static_cast<int>(box.topLeft.x);
    rect.y = static_cast<int>(box.topLeft.y);
    rect.w = static_cast<int>(box.size.width);
    rect.h = static_cast<int>(box.size.height);
  }

}


namespace Engine
{
using namespace std;
using namespace BackEnd;

namespace GraphicDevice
{

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


  // Display
  template <>
  void Display< SDL2::Handler >::render()
  {
    SDL_RenderPresent(_data->_view->_renderer);
  }

  template <>
  void Display< SDL2::Handler >::setColour(const Colour & c)
  {
    this->_background = c;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
    SDL2::colour8RGBA(r, g, b, a, c);

    SDL_SetRenderDrawColor(_data->_view->_renderer, r, g, b, a);
  }

  template <>
  const Colour & Display< SDL2::Handler >::getColour() const
  {
    return this->_background;
  }

  template <>
  void Display< SDL2::Handler >::setTitle(const String & title)
  {
    SDL_SetWindowTitle(_data->_view->_window, title.c_str());
  }

  template <>
  const char * Display< SDL2::Handler >::getTitle() const
  {
    return SDL_GetWindowTitle(_data->_view->_window);
  }

  template <>
  void Display< SDL2::Handler >::clear(const Colour & c)
  {
    this->setColour(c);
    SDL_RenderClear(_data->_view->_renderer);
  }

  template <>
  void Display< SDL2::Handler >::clear()
  {
    Colour c;
    c.kind = RGBA;
    c.rgba = {0.0f, 0.0f, 0.0f, 1.0f};
    setColour(c);
    SDL_RenderClear(_data->_view->_renderer);
  }

  template <>
  void Display< SDL2::Handler >::setFullscreen(bool fs)
  {
    int flag = 0;
    if(fs)
    {
      flag = SDL_WINDOW_FULLSCREEN;
    }

    SDL_SetWindowFullscreen(_data->_view->_window, flag);
    this->setColour(this->_background);
  }

  template <>
  const bool Display< SDL2::Handler >::isFullscreen() const
  {
    return (SDL_GetWindowFlags(_data->_view->_window) == SDL_WINDOW_FULLSCREEN);
  }

  template <>
  void Display< SDL2::Handler >::setResolution(Dimension2 & res)
  {
    // only update if different
    if(_rect.size.width != res.width || _rect.size.height != res.height )
    {
      _rect.size = res;
      int width  = static_cast<int>(_rect.size.width);
      int height = static_cast<int>(_rect.size.height);

      SDL_Rect rect;
      SDL_RenderGetViewport(_data->_view->_renderer, &rect);
      rect.w = width;
      rect.h = height;

      SDL_SetWindowSize(_data->_view->_window, width, height);
      SDL_RenderSetLogicalSize(_data->_view->_renderer, width, height);
      SDL_RenderSetViewport(_data->_view->_renderer, &rect);
      SDL_RenderSetClipRect(_data->_view->_renderer, &rect);
      this->setColour(this->_background);

      _data->_view->_resolution = res;
    }
  }

  template <>
  const Dimension2 & Display< SDL2::Handler >::getResolution() const
  {
    return _rect.size;
  }

  template <>
  void Display< SDL2::Handler >::setViewRect(const BoxBoundXYWH & rect)
  {
    _rect = rect;

    this->setResolution(_rect.size);
    this->setColour(this->_background);
  }

  template <>
  const BoxBoundXYWH & Display< SDL2::Handler >::getViewRect() const
  {
    return _rect;
  }

  template <>
  Display< SDL2::Handler >::Display(const BoxBoundXYWH & rect, Flags flags)
      :_data(new _HANDLER)
      ,_rect(rect)
  {
    this->_background.kind = RGB;
    this->_background.rgb = {0.1, 0.5, 0.9};

    this->setViewRect(rect);
  }

} // end namespace GraphicDevice


namespace Component
{

  // Image
  template <>
  bool Image< SDL2::Handler >::isLoaded()
  {
    return (_data->_image && _data->_image->_buffer);
  }

  template <>
  bool Image< SDL2::Handler >::loadFromFile(const String & filepath)
  {
    _data->_image = SDL2::Texture::createTexture(filepath, _data->_view->_renderer);

    // query window
    int rw = _data->_view->_resolution.width;
    int rh = _data->_view->_resolution.height;

    // query Image
    int tw = _data->_image->_rect.size.width;
    int th = _data->_image->_rect.size.height;

    // compute the relative size
    SpaceDim w =
    static_cast<SpaceDim>(tw) / static_cast<SpaceDim>(rw);
    SpaceDim h =
    static_cast<SpaceDim>(th) / static_cast<SpaceDim>(rh);

    // size - between 0.0 - 1.0 (in relation to Display size)
    _textureSize = {{ w, h }};

    return this->isLoaded();
  }


  template <>
  Image< SDL2::Handler >::Image(const String & filepath)
      :_data(new _HANDLER)
  {
    this->loadFromFile(filepath);
  }

  template <>
  Image< SDL2::Handler >::Image()
      :_data(new _HANDLER)
  {}

  template <>
  Image< SDL2::Handler >::~Image()
  {}

  template <>
  bool Image< SDL2::Handler >::setParameter(
      const char * paramName,
      const float & paramValue)
  {
    return true;
  }

  template <>
  void Image< SDL2::Handler >::computeClipRects(const GraphicPod & component, BoxBoundXYWH & src, BoxBoundXYWH & dst, Vector2 & center)
  {
    int rw = _data->_view->_resolution.width;
    int rh = _data->_view->_resolution.height;

    // compute Image parameters in px
    int tw  = static_cast<int>(_textureSize.width * rw);
    int th  = static_cast<int>(_textureSize.height * rh);
    int tx  = 0;
    int ty  = 0;
    float sx  = 1.0f;
    float sy  = 1.0f;

    if (nullptr != component.transformData)
    {
      tx  = static_cast<int>(component.transformData->position.x);
      ty  = static_cast<int>(component.transformData->position.y);
      sx  = component.transformData->scale.x;
      sy  = component.transformData->scale.y;
    }

    float ax  = component.anchor.x;
    float ay  = component.anchor.y;
    int w = static_cast<int>(tw * abs(sx));
    int h = static_cast<int>(th * abs(sy));
    int cx = static_cast<int>(ax * w);
    int cy = static_cast<int>(ay * h);
    int x = tx - cx;
    int y = ty - cy;

    src.topLeft.x = 0;
    src.topLeft.y = 0;
    src.size.width = tw;
    src.size.height = th;

    dst.topLeft.x = x;
    dst.topLeft.y = y;
    dst.size.width = w;
    dst.size.height = h;

    center.x = cx;
    center.y = cy;
  }

  template <>
  void Image< SDL2::Handler >::paint(const GraphicPod & component, const Vector3 & offset)
  {
    if( _data->_view->_renderer &&
        this->isLoaded() &&
        component.isVisible &&
        component.transformData != nullptr)
    {
      int rw = _data->_view->_resolution.width;
      int rh = _data->_view->_resolution.height;

      BoxBoundXYWH src;
      BoxBoundXYWH dst;
      Vector2 center;

      this->computeClipRects(component, src, dst, center);

      dst.topLeft.x -= static_cast<int>(offset.x);
      dst.topLeft.y -= static_cast<int>(offset.y);

      float rot = 0.0f;
      float sx  = 1.0f;
      float sy  = 1.0f;
      if (nullptr != component.transformData)
      {
        sx  = component.transformData->scale.x;
        sy  = component.transformData->scale.y;
        rot = component.transformData->rotation.yaw;
      }

      double angle = fmod((rot * 360.0), 360.0);
      SDL_Point sdl_center = {static_cast<int>(center.x), static_cast<int>(center.y)};

      if(dst.topLeft.x >= 0.0 && dst.topLeft.y >= 0.0 &&
        dst.topLeft.x <= rw && dst.topLeft.y <= rh)
      {
        // calculate  src and dst rectangles
        SDL_Rect src_rect;
        BoxBoundXYWH2SDLRect(src, src_rect);

        SDL_Rect dst_rect;
        BoxBoundXYWH2SDLRect(dst, dst_rect);

        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (sx < 0.0)
        {
          flip = static_cast<SDL_RendererFlip>( flip | SDL_FLIP_HORIZONTAL);
        }
        if (sy < 0.0)
        {
          flip = static_cast<SDL_RendererFlip>( flip | SDL_FLIP_VERTICAL);
        }

        // blending mode
        SDL_SetTextureBlendMode(_data->_image->_buffer,
            static_cast<SDL_BlendMode>(component.blendingMode));

        // color modulation
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t a = 0;
        SDL2::colour8RGBA(r, g, b, a, component.colourTint);
        SDL_SetTextureColorMod(_data->_image->_buffer, r, g, b);

        // alpha mode
        uint8_t alpha = static_cast<uint8_t>(255 * component.alphaMode);
        SDL_SetTextureAlphaMod(_data->_image->_buffer, alpha);

        // paint the Image
        SDL_RenderCopyEx(_data->_view->_renderer,
                         _data->_image->_buffer,
                         &src_rect,
                         &dst_rect,
                         angle,
                         &sdl_center,
                         flip);
      }
      else
        assert(false);
    }
  }

} // end namespace Component


namespace System
{

  GraphicResourceBinder::GraphicResourceBinder(Component::GraphicInterface * resource, ComponentsHashMap * components)
      : _resource(resource)
      , _components(components)
  {}

  bool GraphicResourceBinder::toEntity(EntityID entityId)
  {
    auto it = this->_components->find(entityId);
    if (it != this->_components->end())
    {
      it->second.resource = _resource;
      return true;
    }

    return false;
  }

  DisplayHandler Graphics::createDisplay(const BoxBoundXYWH & rect, Flags flags)
  {
    this->display = make_shared< GraphicDevice::Display< SDL2::Handler > > (rect, flags);
    return this->display;
  }

  Graphics::Graphics()
      :SystemsInterface("Graphics")
  {
    SDL2::initGraphicSystem();
  }

  Graphics::Graphics(const char * name)
      :SystemsInterface(name)
  {
    SDL2::initGraphicSystem();
  }

  Graphics::~Graphics()
  {
    SDL2::quitGraphicSystem();
  }


  // bool Graphics::setEntityResource(EntityID entityId, int assetId)
  // {
  //   auto itComp = this->_components.find(entityId);
  //   auto itAsset = this->_assets.find(assetId);
  //   if (itComp  != this->_components.end() &&
  //       itAsset != this->_assets.end())
  //   {
  //     itComp->second.element = itAsset->second.get();
  //     return true;
  //   }
  //
  //   return false;
  // }

  void Graphics::insert(Component::EntityPod & entity)
  {
    Component::TransformPod * transform = entity.transform;
    if (transform != nullptr)
    {
      Component::GraphicPod pod;
      this->_components.emplace(entity.entityId, pod);
    }
  }

  void Graphics::remove(const Component::EntityPod & entity)
  {
    auto it = this->_components.find(entity.entityId);
    if (it != this->_components.end())
    {
      this->_components.erase(it);
    }
  }

  void Graphics::tick(TimeDim delta)
  {

  }

  ResourceBinderPtr Graphics::getResourceBinder(ResourceID resourceId)
  {
    ResourceBinderPtr retVal;
    auto it = _resources.find(resourceId);
    if(it != _resources.end())
    {
      retVal.reset(
        new GraphicResourceBinder(it->second.get(), &_components)
      );
    }
    return retVal;
  }

} // end namespace System


} // end namespace Engine
