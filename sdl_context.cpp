#include <cassert>
#include "viewport.hpp"
#include "sdl_context.hpp"

namespace // anonymous
{
  // converts the float values from Colour into SDL RGB8
  void colour8RGBA(uint8_t & r, uint8_t & g, uint8_t & b, uint8_t & a, const Engine::Colour & c)
  {
    r = 0;
    g = 0;
    b = 0;
    a = 255;

    switch (c.kind)
    {
      case Engine::RGB:
      {
        r = (static_cast<uint8_t>(c.rgb.r * 256) % 256);
        g = (static_cast<uint8_t>(c.rgb.g * 256) % 256);
        b = (static_cast<uint8_t>(c.rgb.b * 256) % 256);
      }
      break;
      case Engine::RGBA:
      {
        r = (static_cast<uint8_t>(c.rgba.r * 256) % 256);
        g = (static_cast<uint8_t>(c.rgba.g * 256) % 256);
        b = (static_cast<uint8_t>(c.rgba.b * 256) % 256);
        a = (static_cast<uint8_t>(c.rgba.a * 256) % 256);
      }
      break;
      case Engine::HSL:
      {
        //  TODO
      }
      break;
      case Engine::HSLA:
      {
        //  TODO
      }
      break;
      case Engine::HSV:
      {
        //  TODO
      }
      break;
      case Engine::HSVA:
      {
        //  TODO
      }
      break;
      case Engine::CMYK:
      {
        //  TODO
      }
      break;
      case Engine::HEX:
      {
        //  TODO
      }
      break;
    }
  }
} // end namespace anonymous

namespace Engine
{
using namespace std;

  // here we define classes related to SDL Context;

  // wrapper around window and renderer

  // only one instance of the window is allowed
  SDL_Window * SDLRenderer::_window = nullptr;
  SDL_Renderer * SDLRenderer::_renderer = nullptr;

  SDLRenderer::SDLRenderer()
  {
    if(SDL_WasInit(SDL_INIT_VIDEO) && nullptr == this->_window &&  nullptr == this->_renderer)
    {
      if (  SDL_CreateWindowAndRenderer(
                640,
                480,
                SDL_WINDOW_OPENGL,
                &this->_window,
                &this->_renderer) )
      {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
      }
    }
    assert(this->_window && this->_renderer);
  }

  SDLRenderer::~SDLRenderer()
  {
    if(SDL_WasInit(SDL_INIT_VIDEO) && nullptr == this->_window &&  nullptr == this->_renderer)
    {
      SDL_DestroyRenderer(this->_renderer);
      this->_renderer = nullptr;
      SDL_DestroyWindow(this->_window);
      this->_window = nullptr;
    }
  }

  // wrapper around texture
  SDLTexture::SDLTexture(const string & filepath, SDL_Renderer * renderer)
  {
    /// FIXME: use SDL IMAGE to load tex
    SDL_Surface * surface = SDL_LoadBMP(filepath.c_str());
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
          assert(false);
    }
    else
    {
      this->_buffer = SDL_CreateTextureFromSurface(renderer, surface);
      if (!this->_buffer) {
          SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
      }
      SDL_FreeSurface(surface);
    }
    assert(this->_buffer);
  }

  SDLTexture::~SDLTexture()
  {
    SDL_DestroyTexture(_buffer);
    this->_buffer = nullptr;
  }

  // context used by viewport and texture

  SDLContext::SDLContext()
      :_view(new SDLRenderer())
      ,_image(nullptr)
  {}

  // VIEWPORT
  template <>
  void ViewPort< SDLContext >::render()
  {
    SDL_RenderPresent(_data->_view->_renderer);
  }

  template <>
  void ViewPort< SDLContext >::clear()
  {
    SDL_RenderClear(_data->_view->_renderer);
  }

  template <>
  void ViewPort< SDLContext >::setColour(const Colour & c)
  {
    this->_background = c;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
    colour8RGBA(r, g, b, a, c);

    SDL_SetRenderDrawColor(_data->_view->_renderer, r, g, b, a);
    this->clear();
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
  bool ViewPort< SDLContext >::isFullscreen() const
  {
    return (SDL_GetWindowFlags(_data->_view->_window) == SDL_WINDOW_FULLSCREEN);
  }

  template <>
  void ViewPort< SDLContext >::setResolution(Vector3 & res)
  {
    int width = abs(static_cast<int>(res.x));
    int height = abs(static_cast<int>(res.y));

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

  // FIXME: Implemet this!!!
  // template <>
  // const Vector3 & ViewPort< SDLContext >::getResolution() const
  // {
  //
  //   return vec;
  // }
  //
  // template <>
  // const BoxBoundary & ViewPort< SDLContext >::getViewBox() const;
  // {
  //
  //   return box;
  // }

  template <>
  ViewPort< SDLContext >::ViewPort(BoxBoundary & rect, Flags flags)
      :_data(new Context)
  {
    Vector3 res{
        (rect.buttonRight.x - rect.topLeft.x),
        (rect.buttonRight.y - rect.topLeft.y), 0 };

    this->setResolution(res);
    this->_background.kind = RGB;
    this->_background.rgb = {0.1, 0.5, 0.9};
    this->setColour(this->_background);
  }

  // TEXTURE
  template <>
  Texture< SDLContext >::Texture(GraphicComponent & component)
      :IGraphics()
      ,_component(&component)
      ,_data(new Context)
  {
    _component->texture = this;
  }

  template <>
  Texture< SDLContext >::Texture(GraphicComponent & component, const string & filepath, const string & atlas)
      :IGraphics()
      ,_component(&component)
      ,_data(new Context)
  {
    _component->texture = this;
    _data->_image.reset(new SDLTexture(filepath, _data->_view->_renderer));
    // FIXME: load atlas!!!
  }

  template <>
  Texture< SDLContext >::~Texture()
  {}

  template <>
  bool Texture< SDLContext >::hasImage()
  {
    return (_data->_image && _data->_image->_buffer);
  }

  template <>
  bool Texture< SDLContext >::loadFromFile(const string & filepath, const string & atlas)
  {
    _data->_image.reset(new SDLTexture(filepath, _data->_view->_renderer));
    return this->hasImage();
  }

  // FIXME: Implement this!
  // template <>
  // void Texture< SDLContext >::setPosition(const Vector3 & p)
  // {
  //
  // }
  //
  // const Vector3 & Texture< SDLContext >::getPosition()
  // {
  //   return vec;
  // }

  template <>
  void Texture< SDLContext >::paint(const Vector3 & offset)
  {
    // FIXME: the coordinates have to be changed
    if(_data->_view->_renderer && this->hasImage())
    {
      SDL_RenderCopy(_data->_view->_renderer, _data->_image->_buffer, NULL, NULL);
    }
    else
      assert(false);
  }


} // end namespace Engine
