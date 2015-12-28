/**
  * implement graphic backend functions using SDL
  *
  */

#include "graphic_system_handler.hpp"
#include <SDL2/SDL.h>

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
  bool init();
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

inline bool SDLWrapper::init()
{
  return (SDL_Init(SDL_INIT_NOPARACHUTE) != 0);
}

inline void SDLWrapper::quit()
{
  SDL_Quit();
}

inline bool initSubsystem(const Uint32 flag)
{
  bool wasInit = SDL_WasInit(flag);
  if(!wasInit)
  {
    return SDL_InitSubSystem(flag);
  }
  return wasInit;
}

bool SDLWrapper::initVideo()
{
  return initSubsystem(SDL_INIT_VIDEO);
}

bool SDLWrapper::initEvents()
{
  return initSubsystem(SDL_INIT_EVENTS | SDL_INIT_TIMER);
}
bool SDLWrapper::initInput()
{
  return initSubsystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
}

inline void quitSubsystem(const Uint32 flag)
{
  bool wasInit = SDL_WasInit(flag);
  if(wasInit)
  {
    SDL_QuitSubSystem(flag);
  }
}

void SDLWrapper::quitVideo()
{
  quitSubsystem(SDL_INIT_VIDEO);
}

void SDLWrapper::quitEvents()
{
  quitSubsystem(SDL_INIT_EVENTS | SDL_INIT_TIMER);
}

void SDLWrapper::quitInput()
{
  quitSubsystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
}

} // end namespace anonymous


namespace Engine
{
using namespace std;

// implement handler interface using SDL
template< class Texture >
errorCode GraphicsHandler::LoadTexture(Texture* tex, const char * filepath, const char * atlas)
{
  return 0;
}

template< class ViewPort >
errorCode GraphicsHandler::CreateViewPort(ViewPort * view, BoxBoundary & rect, uint8_t flags)
{
  return 0;
}

// grahics functions
bool initGraphicSystem(uint flags)
{
  return SDLWrapper::instance()->initVideo();
}

void quitGraphicSystem()
{
  SDLWrapper::instance()->quitVideo();
}

errorCode getGraphicHandler(IHandler * handler, const IConfig * data)
{
  return 0;
}

// inputs functions
bool initInputSystem(uint flags)
{
  return SDLWrapper::instance()->initInput();
}

void quitInputSystem()
{
  SDLWrapper::instance()->quitInput();
}

errorCode getInputHandler(IHandler * handler, const IConfig * data)
{
  return 0;
}

// events functions
bool initEventSystem(uint flags)
{
  return SDLWrapper::instance()->initEvents();
}

void quitEventSystem()
{
  SDLWrapper::instance()->quitEvents();
}

errorCode getEventHandler(IHandler * handler, const IConfig * data)
{
  return 0;
}

} // end namespace Engine
