/**
  * implement graphic backend functions using SDL
  *
  */

#include <cassert>
#include <cmath>
#include <SDL2/SDL.h>

//#include "graphic_system_handler.hpp"
#include "backend.hpp"
#include "viewport.hpp"


namespace Engine
{
using namespace std;

// here we define classes related to SDL Context;

// wrapper around window and renderer
struct SDLRenderer
{
  SDLRenderer();
  ~SDLRenderer();
  static SDL_Window * _window;
  static SDL_Renderer * _renderer;
};

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
struct SDLTexture
{
  SDLTexture(const string & filepath, SDL_Renderer * renderer);
  ~SDLTexture();
  SDL_Texture * _texture;
};

SDLTexture::SDLTexture(const string & filepath, SDL_Renderer * renderer)
{
  /// FIXME: use SDL IMAGE to load tex
  SDL_Surface * surface = SDL_LoadBMP(filepath.c_str());
  if (!surface) {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
  }
  else
  {
    this->_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!this->_texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
    }
    SDL_FreeSurface(surface);
  }
  assert(this->_texture);
}

SDLTexture::~SDLTexture()
{
  SDL_DestroyTexture(_texture);
  this->_texture = nullptr;
}


// context used by viewport and texture
struct SDLContext
{
  SDLContext();

  SDLRenderer _view;
  unique_ptr< SDLTexture > _texture;
};

SDLContext::SDLContext()
    :_texture(nullptr)
{}

} // end namespace Engine


// this anonymous (restricted) namespace contains a
// singleton class that inits SDL and its subsystems
namespace // anonymous
{
  class SDLWrapper
  {
  public:
    ~SDLWrapper();

    static SDLWrapper * instance();

    bool initVideo();
    bool initEvents();
    bool initInput();

    void quitVideo();
    void quitEvents();
    void quitInput();

  protected:
    SDLWrapper();
    void init();
    void quit();
  };

  inline SDLWrapper * SDLWrapper::instance()
  {
    static SDLWrapper * s_instance = nullptr;

    if(nullptr == s_instance)
    {
      s_instance = new SDLWrapper();
    }
    return s_instance;
  }

  inline SDLWrapper::SDLWrapper()
  {
    this->init();
  }

  inline SDLWrapper::~SDLWrapper()
  {
    this->quit();
  }

  inline void SDLWrapper::init()
  {
    int initResult = SDL_Init(SDL_INIT_NOPARACHUTE);
    if (initResult < 0)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    }
    assert(initResult == 0);
  }

  inline void SDLWrapper::quit()
  {
    SDL_Quit();
  }

  inline bool initSubsystem(const Engine::Flags flag)
  {
    bool wasInit = SDL_WasInit(flag);
    if(!wasInit)
    {
      return SDL_InitSubSystem(flag);
    }
    return wasInit;
  }

  inline bool SDLWrapper::initVideo()
  {
    return initSubsystem(SDL_INIT_VIDEO);
  }

  inline bool SDLWrapper::initEvents()
  {
    return initSubsystem(SDL_INIT_EVENTS | SDL_INIT_TIMER);
  }

  inline bool SDLWrapper::initInput()
  {
    return initSubsystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
  }

  inline void quitSubsystem(const Engine::Flags flag)
  {
    bool wasInit = SDL_WasInit(flag);
    if(wasInit)
    {
      SDL_QuitSubSystem(flag);
    }
  }

  inline void SDLWrapper::quitVideo()
  {
    quitSubsystem(SDL_INIT_VIDEO);
  }

  inline void SDLWrapper::quitEvents()
  {
    quitSubsystem(SDL_INIT_EVENTS | SDL_INIT_TIMER);
  }

  inline void SDLWrapper::quitInput()
  {
    quitSubsystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
  }


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


// rendering
template <>
void ViewPort< SDLContext >::paint(Texture< SDLContext > & tex)
{
  //SDL_RenderCopy(_data->_view._renderer, texture, NULL, NULL);
}

template <>
void ViewPort< SDLContext >::render()
{
  SDL_RenderPresent(_data->_view._renderer);
}

template <>
void ViewPort< SDLContext >::clear()
{
  SDL_RenderClear(_data->_view._renderer);
  this->render();
}

template <>
void ViewPort< SDLContext >::setColour(const Colour & c)
{
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  uint8_t a = 0;
  colour8RGBA(r, g, b, a, c);

  SDL_SetRenderDrawColor(_data->_view._renderer, r, g, b, a);
  this->clear();
}

template <>
void ViewPort< SDLContext >::setFullscreen(bool fs)
{
  SDL_SetWindowFullscreen(_data->_view._window, SDL_WINDOW_FULLSCREEN);
}

template <>
bool ViewPort< SDLContext >::isFullscreen() const
{

  return (SDL_GetWindowFlags(_data->_view._window) == SDL_WINDOW_FULLSCREEN);
}

template <>
void ViewPort< SDLContext >::setResolution(Vector3 & res)
{
  int width = abs(static_cast<int>(res.x));
  int height = abs(static_cast<int>(res.y));

  SDL_SetWindowSize(_data->_view._window, width, height);
  SDL_RenderSetLogicalSize(_data->_view._renderer, width, height);
}

// template <>
// const Vector3 & ViewPort< SDLContext >::getResolution() const
// {
//
//   return vec;
// }
//
// template <>
// const BoxBoundary & ViewPort< SDLContext >::getView() const;
// {
//
//   return box;
// }

template <>
ViewPort< SDLContext >::ViewPort(BoxBoundary & rect, Flags flags)
    :_data(new Context)
{
  Vector3 res{(rect.buttonRight.x - rect.topLeft.x),
      (rect.buttonRight.y - rect.topLeft.y), 0};
  this->setResolution(res);
  SDL_SetRenderDrawColor(_data->_view._renderer, 0x00, 0x00, 0x00, 0x00);
  this->clear();
}

template <>
Texture< SDLContext >::Texture(GraphicComponent & component)
{

}

template <>
Texture< SDLContext >::Texture(GraphicComponent & component, const string & filepath, const string & atlas)
{

}

template <>
Texture< SDLContext >::~Texture()
{

}

template <>
bool Texture< SDLContext >::loadFromFile(const string & filepath, const string & atlas)
{

  return false;
}

template <>
bool Texture< SDLContext >::hasBMP()
{

  return false;
}

template <>
void Texture< SDLContext >::setPosition(const Vector3 & p)
{

}

// const Vector3 & Texture< SDLContext >::getPosition()
// {
//   return vec;
// }


// // implement handler interface using SDL
// //template< class Impl >
// template<>
// ErrorCode
// GraphicSystemHandler< SDLContext >::CreateViewPort(ViewPort< SDLContext > * view, BoxBoundary & rect, Flags flags)
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

// grahics functions
bool initGraphicSystem(Flags flags)
{
  return SDLWrapper::instance()->initVideo();
}

void quitGraphicSystem()
{
  SDLWrapper::instance()->quitVideo();
}

ErrorCode getGraphicHandler(IHandler * handler, const IConfig * data)
{
  return 0;
}

// inputs functions
bool initInputSystem(Flags flags)
{
  return SDLWrapper::instance()->initInput();
}

void quitInputSystem()
{
  SDLWrapper::instance()->quitInput();
}

ErrorCode getInputHandler(IHandler * handler, const IConfig * data)
{
  return 0;
}

// events functions
bool initEventSystem(Flags flags)
{
  return SDLWrapper::instance()->initEvents();
}

void quitEventSystem()
{
  SDLWrapper::instance()->quitEvents();
}

ErrorCode getEventHandler(IHandler * handler, const IConfig * data)
{
  return 0;
}

} // end namespace Engine
