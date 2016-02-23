#include "backends/handler_sdl.hpp"
#include <SDL2/SDL_image.h>

namespace // anonymous
{
// holds pointers to loaded textures to avoid reloading textures more than once.

using namespace W2E::BE;
using namespace W2E::Utils;

INTERNAL HashMap< String, SharedPtr< SDL2::Texture > > textures;

// this anonymous (restricted) namespace contains a
// singleton class that inits SDL and its subsystems

class SDLWrapper
{
public:
  ~SDLWrapper();

  CLASS_METHOD SDLWrapper* instance();

  ErrorCode initVideo();
  ErrorCode initEvents();
  ErrorCode initInput();

  void quitVideo();
  void quitEvents();
  void quitInput();

protected:
  SDLWrapper();
  void init();
  void quit();
};

inline SDLWrapper* SDLWrapper::instance()
{
  LOCAL_PERSISTENT SDLWrapper* sinstance_ = nullptr;

  if(nullptr == sinstance_)
  {
    sinstance_ = new SDLWrapper();
  }
  return sinstance_;
}

inline SDLWrapper::SDLWrapper() { this->init(); }

inline SDLWrapper::~SDLWrapper() { this->quit(); }

inline void SDLWrapper::init()
{
  int initSdl = SDL_Init(SDL_INIT_NOPARACHUTE);
  if(initSdl < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
  }
  assert(initSdl == 0);

  int flags = IMG_INIT_PNG;
  int initImg = IMG_Init(flags);
  if(initImg < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL Image: %s", SDL_GetError());
  }
  assert(initImg == flags);
}

inline void SDLWrapper::quit()
{
  IMG_Quit();
  SDL_Quit();
}

inline ErrorCode initSubsystem(const W2E::Flags flag)
{
  if(!SDL_WasInit(flag))
  {
    return (SDL_InitSubSystem(flag)) ? NO_ERROR : UNKNOWN_ERROR;
  }
  return UNKNOWN_ERROR;
}

inline ErrorCode SDLWrapper::initVideo() { return initSubsystem(SDL_INIT_VIDEO); }

inline ErrorCode SDLWrapper::initEvents()
{
  return initSubsystem(SDL_INIT_EVENTS | SDL_INIT_TIMER);
}

inline ErrorCode SDLWrapper::initInput()
{
  return initSubsystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
}

inline void quitSubsystem(const W2E::Flags flag)
{
  if(SDL_WasInit(flag))
  {
    SDL_QuitSubSystem(flag);
  }
}

inline void SDLWrapper::quitVideo() { quitSubsystem(SDL_INIT_VIDEO); }

inline void SDLWrapper::quitEvents() { quitSubsystem(SDL_INIT_EVENTS | SDL_INIT_TIMER); }

inline void SDLWrapper::quitInput() { quitSubsystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK); }

} // end namespace anonymous

namespace W2E
{

using namespace Utils;

namespace BE
{

namespace SDL2
{

// converts the float values from Colour into SDL RGB8
void colour8RGBA(u8& r, u8& g, u8& b, u8& a, const W2E::Colour& c)
{
  r = 0;
  g = 0;
  b = 0;
  a = 255;

  switch(c.kind)
  {
  case W2E::RGB:
  {
    r = (static_cast< u8 >(c.rgb.r * 255) % 256);
    g = (static_cast< u8 >(c.rgb.g * 255) % 256);
    b = (static_cast< u8 >(c.rgb.b * 255) % 256);
    break;
  }
  case W2E::RGBA:
  {
    r = (static_cast< u8 >(c.rgba.r * 255) % 256);
    g = (static_cast< u8 >(c.rgba.g * 255) % 256);
    b = (static_cast< u8 >(c.rgba.b * 255) % 256);
    a = (static_cast< u8 >(c.rgba.a * 255) % 256);
    break;
  }
  case W2E::HSL:
  {
    ColourRGB tmp;
    Hsl2Rgb(c.hsl, tmp);
    r = (static_cast< u8 >(tmp.r * 255) % 256);
    g = (static_cast< u8 >(tmp.g * 255) % 256);
    b = (static_cast< u8 >(tmp.b * 255) % 256);
    break;
  }
  case W2E::HSLA:
  {
    ColourRGB tmp;
    Hsl2Rgb({c.hsla.h, c.hsla.s, c.hsla.l}, tmp);
    r = (static_cast< u8 >(tmp.r * 255) % 256);
    g = (static_cast< u8 >(tmp.g * 255) % 256);
    b = (static_cast< u8 >(tmp.b * 255) % 256);
    a = (static_cast< u8 >(c.hsla.a * 255) % 256);
  }
  break;
  case W2E::HSV:
  {
    ColourRGB tmp;
    Hsv2Rgb(c.hsv, tmp);
    r = (static_cast< u8 >(tmp.r * 255) % 256);
    g = (static_cast< u8 >(tmp.g * 255) % 256);
    b = (static_cast< u8 >(tmp.b * 255) % 256);
    break;
  }
  case W2E::HSVA:
  {
    ColourRGB tmp;
    Hsl2Rgb({c.hsva.h, c.hsva.s, c.hsva.v}, tmp);
    r = (static_cast< u8 >(tmp.r * 255) % 256);
    g = (static_cast< u8 >(tmp.g * 255) % 256);
    b = (static_cast< u8 >(tmp.b * 255) % 256);
    a = (static_cast< u8 >(c.hsva.a * 255) % 256);
  }
  break;
  case W2E::CMYK:
  {
    ColourRGB tmp;
    Cmyk2Rgb(c.cmyk, tmp);
    r = (static_cast< u8 >(tmp.r * 255) % 256);
    g = (static_cast< u8 >(tmp.g * 255) % 256);
    b = (static_cast< u8 >(tmp.b * 255) % 256);
    break;
  }
  case W2E::HEX:
  {
    //  TODO
    break;
  }
  }
}

// here we define classes related to SDL Handler;

// wrapper around window and renderer

// only one instance of the window is allowed
Dimension2 Renderer::resolution_ = {{640.0, 480.0}};
SDL_Window* Renderer::window_ = nullptr;
SDL_Renderer* Renderer::renderer_ = nullptr;

Renderer::Renderer()
{
  if(SDL_WasInit(SDL_INIT_VIDEO) && nullptr == this->window_ && nullptr == this->renderer_)
  {
    if(SDL_CreateWindowAndRenderer(static_cast< int >(resolution_.width),
                                   static_cast< int >(resolution_.height),
                                   SDL_WINDOW_OPENGL,
                                   &this->window_,
                                   &this->renderer_))
    {
      SDL_LogError(
          SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
    }
  }
  assert(this->window_ && this->renderer_);
}

Renderer::~Renderer()
{
  if(SDL_WasInit(SDL_INIT_VIDEO) && nullptr == this->window_ && nullptr == this->renderer_)
  {
    SDL_DestroyRenderer(this->renderer_);
    this->renderer_ = nullptr;
    SDL_DestroyWindow(this->window_);
    this->window_ = nullptr;
  }
}

// wrapper around texture
Texture::Texture(const String& filepath, SDL_Renderer* renderer)
{
  SDL_Surface* surface = IMG_Load(filepath.c_str());
  if(!surface)
  {
    SDL_LogError(
        SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
    assert(false);
  }
  else
  {
    this->buffer_ = SDL_CreateTextureFromSurface(renderer, surface);
    if(!this->buffer_)
    {
      SDL_LogError(
          SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
    }
    rect_.topLeft = {{0.0f, 0.0f}};
    rect_.size = {{static_cast< SpaceDim >(surface->w), static_cast< SpaceDim >(surface->h)}};
    SDL_FreeSurface(surface);
  }
  assert(this->buffer_);
}

SharedPtr< Texture > Texture::createTexture(const String& filepath, SDL_Renderer* renderer)
{
  SharedPtr< Texture > texPtr(nullptr);

  auto sdlTexIt = textures.find(filepath);
  if(sdlTexIt != textures.end())
  {
    texPtr = sdlTexIt->second;
  }
  else
  {

    texPtr.reset(new Texture(filepath, renderer));
    textures[filepath] = texPtr;
  }
  return texPtr;
}

Texture::~Texture()
{
  // if this texture was shared it might have been destroye already!
  if(nullptr != this->buffer_)
  {
    SDL_DestroyTexture(this->buffer_);
    this->buffer_ = nullptr;
  }
}

// context used by viewport and texture

Handler::Handler()
    : view_(new Renderer())
    , image_(nullptr)
{
}

// graphic functions
ErrorCode initGraphicSystem(Flags flags) { return SDLWrapper::instance()->initVideo(); }

void quitGraphicSystem() { SDLWrapper::instance()->quitVideo(); }

// input functions
ErrorCode initInputSystem(Flags flags)
{
  return static_cast< ErrorCode >(SDLWrapper::instance()->initEvents() |
                                  SDLWrapper::instance()->initInput());
}

void quitInputSystem()
{
  SDLWrapper::instance()->quitEvents();
  SDLWrapper::instance()->quitInput();
}

} // end namespace SDL2

} // end namespace BE

} // end namespace W2E
