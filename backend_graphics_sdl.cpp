/**
  * implement graphic backend functions using SDL
  *
  */

#include <cassert>
#include <cmath>

#include "backend.hpp"
#include "context_sdl.hpp"
#include "viewport.hpp"
//#include "graphic_system_handler.hpp"


namespace Engine
{
using namespace std;

  // // implement handler interface using SDL
  // //template< class Impl >
  // template<>
  // ErrorCode
  // GraphicSystemHandler< SDLContext >::CreateViewPort(ViewPort< SDLContext > * view, BoxBoundXYWH & rect, Flags flags)
  // {
  //   bool wasInit = SDL_WasInit(SDL_INIT_VIDEO);
  //   if (wasInit)
  //   {
  //
  //   }
  //   return -1;
  // }
  //
  // template< class Impl >
  //   ErrorCode
  //   GraphicSystemHandler< Impl >::LoadTexture(Texture* tex, const char * filepath, const char * atlas)
  // {
  //   return 0;
  // }

  // graphic functions
  ErrorCode getGraphicHandler(IHandler * handler, const IConfig * data)
  {
    return 0;
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

  // VIEWPORT
  template <>
  void ViewPort< SDLContext >::render()
  {
    SDL_RenderPresent(_data->_view->_renderer);
  }

  template <>
  void ViewPort< SDLContext >::setColour(const Colour & c)
  {
    this->_background = c;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
    SDLBackEnd::colour8RGBA(r, g, b, a, c);

    SDL_SetRenderDrawColor(_data->_view->_renderer, r, g, b, a);
  }

  template <>
  const Colour & ViewPort< SDLContext >::getColour() const
  {
    return this->_background;
  }

  template <>
  void ViewPort< SDLContext >::setTitle(const string & title)
  {
    SDL_SetWindowTitle(_data->_view->_window, title.c_str());
  }

  template <>
  const char * ViewPort< SDLContext >::getTitle() const
  {
    return SDL_GetWindowTitle(_data->_view->_window);
  }

  template <>
  void ViewPort< SDLContext >::clear(const Colour * c)
  {
    if(nullptr != c)
    {
      setColour(*c);
    }
    SDL_RenderClear(_data->_view->_renderer);
  }

  template <>
  void ViewPort< SDLContext >::setFullscreen(bool fs)
  {
    if(fs)
    {
      SDL_SetWindowFullscreen(_data->_view->_window, SDL_WINDOW_FULLSCREEN);
      this->setColour(this->_background);
    }
  }

  template <>
  const bool ViewPort< SDLContext >::isFullscreen() const
  {
    return (SDL_GetWindowFlags(_data->_view->_window) == SDL_WINDOW_FULLSCREEN);
  }

  template <>
  void ViewPort< SDLContext >::setResolution(Dimension3 & res)
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
    }
  }

  template <>
  const Dimension3 & ViewPort< SDLContext >::getResolution() const
  {
    return _rect.size;
  }

  template <>
  void ViewPort< SDLContext >::setViewRect(const BoxBoundXYWH & rect)
  {
    _rect = rect;

    this->setResolution(_rect.size);
    this->setColour(this->_background);
  }

  template <>
  const BoxBoundXYWH & ViewPort< SDLContext >::getViewRect() const
  {
    return _rect;
  }

  template <>
  ViewPort< SDLContext >::ViewPort(const BoxBoundXYWH & rect, Flags flags)
      :_data(new Context)
      ,_rect(rect)
  {
    this->_background.kind = RGB;
    this->_background.rgb = {0.1, 0.5, 0.9};

    this->setViewRect(rect);
  }

  // TEXTURE
  template <>
  Texture< SDLContext >::Texture(GraphicComponent & component)
      :_component(&component)
      ,_data(new Context)
  {
    _component->texture = this;
  }

  template <>
  Texture< SDLContext >::Texture(GraphicComponent & component, const string & filepath, const string & atlas)
      :_component(&component)
      ,_data(new Context)
  {
    _component->texture = this;
    _data->_image.reset(new SDLTexture(filepath, _data->_view->_renderer));
  }

  template <>
  Texture< SDLContext >::~Texture()
  {}

  template <>
  bool Texture< SDLContext >::isLoaded()
  {
    return (_data->_image && _data->_image->_buffer);
  }

  template <>
  bool Texture< SDLContext >::loadFromFile(const string & filepath, const string & atlas)
  {
    _data->_image.reset(new SDLTexture(filepath, _data->_view->_renderer));
    return this->isLoaded();
  }

  template <>
  void Texture< SDLContext >::paint(const Vector3 & offset)
  {
    // FIXME: the coordinates have to be changed
    // use: int SDL_RenderCopyEx(SDL_Renderer* renderer,
    //                  SDL_Texture*           texture,
    //                  const SDL_Rect*        srcrect,
    //                  const SDL_Rect*        dstrect,
    //                  const double           angle,
    //                  const SDL_Point*       center,
    //                  const SDL_RendererFlip flip);
    // need to get
    // void SDL_GetWindowSize(SDL_Window* window,
    //                   int*        w,
    //                   int*        h);
    // void SDL_RenderGetLogicalSize(SDL_Renderer* renderer,
    //                   int*        w,
    //                   int*        h);
    // then calculate the actual position.
    // CAVEAT: origin will be changed from topleft to bottonleft!

    if(_data->_view->_renderer && this->isLoaded())
    {
      SDL_RenderCopy(_data->_view->_renderer, _data->_image->_buffer, NULL, NULL);
    }
    else
      assert(false);
  }


} // end namespace Engine
