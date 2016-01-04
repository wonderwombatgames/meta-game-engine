/**
  * implement graphic backend functions using SDL
  *
  */

#include <cassert>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "backend.hpp"
#include "backend_context_sdl.hpp"


namespace
{
using namespace Engine;
using namespace Utils;

  inline void BoxBoundXYWH2SDLRect(const BoxBoundXYWH & box, SDL_Rect & rect)
  {
    rect.x = static_cast<int>(box.topLeft.x);
    rect.y = static_cast<int>(box.topLeft.y);
    rect.w = static_cast<int>(box.size.w);
    rect.h = static_cast<int>(box.size.h);
  }

}


namespace Engine
{
using namespace std;
using namespace BackEnd;

  // VIEWPORT
  template <>
  void ViewPort< SDL2::Context >::render()
  {
    SDL_RenderPresent(_data->_view->_renderer);
  }

  template <>
  void ViewPort< SDL2::Context >::setColour(const Colour & c)
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
  const Colour & ViewPort< SDL2::Context >::getColour() const
  {
    return this->_background;
  }

  template <>
  void ViewPort< SDL2::Context >::setTitle(const string & title)
  {
    SDL_SetWindowTitle(_data->_view->_window, title.c_str());
  }

  template <>
  const char * ViewPort< SDL2::Context >::getTitle() const
  {
    return SDL_GetWindowTitle(_data->_view->_window);
  }

  template <>
  void ViewPort< SDL2::Context >::clear(const Colour * c)
  {
    if(nullptr != c)
    {
      setColour(*c);
    }
    SDL_RenderClear(_data->_view->_renderer);
  }

  template <>
  void ViewPort< SDL2::Context >::setFullscreen(bool fs)
  {
    if(fs)
    {
      SDL_SetWindowFullscreen(_data->_view->_window, SDL_WINDOW_FULLSCREEN);
      this->setColour(this->_background);
    }
  }

  template <>
  const bool ViewPort< SDL2::Context >::isFullscreen() const
  {
    return (SDL_GetWindowFlags(_data->_view->_window) == SDL_WINDOW_FULLSCREEN);
  }

  template <>
  void ViewPort< SDL2::Context >::setResolution(Dimension3 & res)
  {
    // only update if different
    if(_rect.size.w != res.w || _rect.size.h != res.h )
    {
      _rect.size = res;
      int width  = static_cast<int>(_rect.size.w);
      int height = static_cast<int>(_rect.size.h);

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
  const Dimension3 & ViewPort< SDL2::Context >::getResolution() const
  {
    return _rect.size;
  }

  template <>
  void ViewPort< SDL2::Context >::setViewRect(const BoxBoundXYWH & rect)
  {
    _rect = rect;

    this->setResolution(_rect.size);
    this->setColour(this->_background);
  }

  template <>
  const BoxBoundXYWH & ViewPort< SDL2::Context >::getViewRect() const
  {
    return _rect;
  }

  template <>
  ViewPort< SDL2::Context >::ViewPort(const BoxBoundXYWH & rect, Flags flags)
      :_data(new Context)
      ,_rect(rect)
  {
    this->_background.kind = RGB;
    this->_background.rgb = {0.1, 0.5, 0.9};

    this->setViewRect(rect);
  }

  // TEXTURE
  template <>
  void Texture< SDL2::Context >::init()
  {
    // defines the anchor within the boudaries
    // values between 0.0 - 1.0 (in relation to entity size | UV)
    _component->anchor = {0.0f, 0.0f, 0.0f};

    // size - between 0.0 - 1.0 (in relation to viewport size)
    _component->textureSize = {0.0f, 0.0f, 0.0f};

    // texture data pointer
    _component->texture = this;

    // nth frame in within the texture atlas
    _component->animationFrame = 0;

    // colour parameters
    _component->colourTint.kind = RGB;
    _component->colourTint.rgb = {1.0f, 1.0f, 1.0f};
    _component->alphaMode = 1.0f;
    _component->blendingMode = SDL_BLENDMODE_BLEND;

    // whether to show the entity or not
    _component->isVisible = true;

  }

  template <>
  Texture< SDL2::Context >::Texture(GraphicComponent & component)
      :_component(&component)
      ,_data(new Context)
  {
    this->init();
  }

  template <>
  bool Texture< SDL2::Context >::isLoaded()
  {
    return (_data->_image && _data->_image->_buffer);
  }

  template <>
  inline void Texture< SDL2::Context >::getWindowSize(int & w, int & h)
  {
    w = _data->_view->_resolution.w;
    h = _data->_view->_resolution.h;
  }

  template <>
  inline void Texture< SDL2::Context >::getTextureSize(int & w, int & h)
  {
    w = _data->_image->_rect.size.w;
    h = _data->_image->_rect.size.h;
  }

  template <>
  bool Texture< SDL2::Context >::loadFromFile(const string & filepath)
  {
    _data->_image = SDL2::Texture::createTexture(filepath, _data->_view->_renderer);

    // query window
    int rw = 0;
    int rh = 0;
    this->getWindowSize(rw, rh);

    // query texture
    int tw = 0;
    int th = 0;
    this->getTextureSize(tw, th);

    // compute the relative size
    SpatialDimention w =
    static_cast<SpatialDimention>(tw) / static_cast<SpatialDimention>(rw);
    SpatialDimention h =
    static_cast<SpatialDimention>(th) / static_cast<SpatialDimention>(rh);

    // size - between 0.0 - 1.0 (in relation to viewport size)
    _component->textureSize = {w, h, 0.0f};

    return this->isLoaded();
  }

  template <>
  Texture< SDL2::Context >::Texture(GraphicComponent & component, const string & filepath)
      :_component(&component)
      ,_data(new Context)
  {
    this->init();
    this->loadFromFile(filepath);
  }

  template <>
  Texture< SDL2::Context >::~Texture()
  {}

  template <>
  void Texture< SDL2::Context >::computeClipRects(
      BoxBoundXYWH & src, BoxBoundXYWH & dst, Vector3 & center)
  {
    int rw = 0;
    int rh = 0;
    this->getWindowSize(rw, rh);

    // compute texture parameters in px
    int tw  = static_cast<int>(_component->textureSize.w * rw);
    int th  = static_cast<int>(_component->textureSize.h * rh);
    int tx  = 0;
    int ty  = 0;
    float sx  = 1.0f;
    float sy  = 1.0f;

    if (nullptr != _component->transformData)
    {
      tx  = static_cast<int>(_component->transformData->position.x);
      ty  = static_cast<int>(_component->transformData->position.y);
      sx  = _component->transformData->scale.x;
      sy  = _component->transformData->scale.y;
    }

    float ax  = _component->anchor.x;
    float ay  = _component->anchor.y;
    int w = static_cast<int>(tw * abs(sx));
    int h = static_cast<int>(th * abs(sy));
    int cx = static_cast<int>(ax * w);
    int cy = static_cast<int>(ay * h);
    int x = tx - cx;
    int y = ty - cy;

    src.topLeft.x = 0;
    src.topLeft.y = 0;
    src.size.w = tw;
    src.size.h = th;

    dst.topLeft.x = x;
    dst.topLeft.y = y;
    dst.size.w = w;
    dst.size.h = h;

    center.x = cx;
    center.y = cy;
  }

  template <>
  void Texture< SDL2::Context >::paint(const Vector3 & offset)
  {
    if(_data->_view->_renderer && this->isLoaded() && _component->isVisible)
    {
      int rw = 0;
      int rh = 0;
      this->getWindowSize(rw, rh);

      BoxBoundXYWH src;
      BoxBoundXYWH dst;
      Vector3 center;

      this->computeClipRects(src, dst, center);

      dst.topLeft.x -= static_cast<int>(offset.x);
      dst.topLeft.y -= static_cast<int>(offset.y);

      float rot = 0.0f;
      float sx  = 1.0f;
      float sy  = 1.0f;
      if (nullptr != _component->transformData)
      {
        sx  = _component->transformData->scale.x;
        sy  = _component->transformData->scale.y;
        rot = _component->transformData->rotation.angleXY;
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
            static_cast<SDL_BlendMode>(_component->blendingMode));

        // color modulation
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t a = 0;
        SDL2::colour8RGBA(r, g, b, a, _component->colourTint);
        SDL_SetTextureColorMod(_data->_image->_buffer, r, g, b);

        // alpha mode
        uint8_t alpha = static_cast<uint8_t>(255 * _component->alphaMode);
        SDL_SetTextureAlphaMod(_data->_image->_buffer, alpha);

        // paint the texture
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


  template<>
  GraphicSystemHandler< SDL2::Context >::GraphicSystemHandler()
  {
    SDL2::initGraphicSystem();
  }

  template<>
  GraphicSystemHandler< SDL2::Context >::~GraphicSystemHandler()
  {
    SDL2::quitGraphicSystem();
  }

  template<>
  SDL2BackEnd::ViewPortPtr GraphicSystemHandler< SDL2::Context >::getViewPort()
  {
    return SDL2BackEnd::ViewPortPtr(
        new SDL2BackEnd::ViewPort({{0.0,0.0,0.0}, {640.0, 480.0, 0.0}}) );
  }

namespace BackEnd
{

  // graphic functions
  ErrorCode getGraphicHandler(IHandler * handler, const IConfig * data)
  {
    handler = new SDL2BackEnd::GraphicSysHandler();
    if(nullptr != handler)
    {
      return 0;
    }
    else
    {
      return -1;
    }
  }

  //
  // // inputs functions
  // ErrorCode getInputHandler(IHandler * handler, const IConfig * data)
  // {
  //   return 0;
  // }
  //
  // // events functions
  // ErrorCode getEventHandler(IHandler * handler, const IConfig * data)
  // {
  //   return 0;
  // }


} // end namespace BackEnd


} // end namespace Engine
