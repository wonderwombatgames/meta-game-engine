/**
  * implement graphic backend functions using SDL
  *
  */

#include <cassert>
#include <cmath>

#include "backend.hpp"
#include "sdl_context.hpp"
//#include "graphic_system_handler.hpp"


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

} // end namespace anonymous


namespace Engine
{
using namespace std;

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
