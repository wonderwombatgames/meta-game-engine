#include <cassert>
#include <unordered_map>
#include <SDL2/SDL_image.h>
#include "context_sdl.hpp"
#include "basic_types.hpp"
#include "backend.hpp"


namespace // anonymous
{
  // holds pointers to loaded textures to avoid reloading textures more than once.
  static std::unordered_map<std::string, Engine::SDLTexture * > textures;

  // this anonymous (restricted) namespace contains a
  // singleton class that inits SDL and its subsystems

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

} // end namespace anonymous


namespace Engine
{
using namespace std;

namespace SDLBackEnd
{
  // graphic functions
  bool initGraphicSystem(Flags flags)
  {
    return SDLWrapper::instance()->initVideo();
  }

  void quitGraphicSystem()
  {
    SDLWrapper::instance()->quitVideo();
  }

  // // inputs functions
  // bool initInputSystem(Flags flags)
  // {
  //   return SDLWrapper::instance()->initInput();
  // }
  //
  // void quitInputSystem()
  // {
  //   SDLWrapper::instance()->quitInput();
  // }
  //
  // // events functions
  // bool initEventSystem(Flags flags)
  // {
  //   return SDLWrapper::instance()->initEvents();
  // }
  //
  // void quitEventSystem()
  // {
  //   SDLWrapper::instance()->quitEvents();
  // }

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
} // end namespace SDLBackEnd

  // here we define classes related to SDL Context;

  // wrapper around window and renderer

  // only one instance of the window is allowed
  Dimension3 SDLRenderer::_resolution = {640.0, 480.0};
  SDL_Window * SDLRenderer::_window = nullptr;
  SDL_Renderer * SDLRenderer::_renderer = nullptr;

  SDLRenderer::SDLRenderer()
  {
    if(SDL_WasInit(SDL_INIT_VIDEO) && nullptr == this->_window &&  nullptr == this->_renderer)
    {
      if (  SDL_CreateWindowAndRenderer(
                static_cast<int>(_resolution.w),
                static_cast<int>(_resolution.h),
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
    SDL_Surface * surface = IMG_Load(filepath.c_str());
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
      _rect.topLeft = {0.0f, 0.0f};
      _rect.size = {static_cast<SpatialDimention>(surface->w),
                    static_cast<SpatialDimention>(surface->h)};
      SDL_FreeSurface(surface);
    }
    assert(this->_buffer);
  }

  SDLTexture * SDLTexture::createSDLTexture(const string & filepath, SDL_Renderer * renderer)
  {
    SDLTexture * texPtr = nullptr;

    auto sdlTexIt = textures.find(filepath);
    if(sdlTexIt != textures.end())
    {
      texPtr = sdlTexIt->second;
    }
    else
    {
      texPtr = new SDLTexture(filepath, renderer);
      textures[filepath] = texPtr;
    }
    return texPtr;
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


} // end namespace Engine
