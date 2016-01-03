/**
  * SDL context to use in the implementation of Texture and Viewport
  * All other contexts should follow more or less the same structure;
  */

#ifndef SDL_CONTEXT_HPP
#define SDL_CONTEXT_HPP

#include <memory>
#include <SDL2/SDL.h>
#include "basic_types.hpp"
#include "viewport.hpp"
#include "graphic_system_handler.hpp"

namespace Engine
{
using namespace std;

namespace SDLBackEnd
{

  // wrapper around window and renderer
  struct SDLRenderer
  {
    SDLRenderer();
    ~SDLRenderer();
    static Dimension3 _resolution;
    static SDL_Window * _window;
    static SDL_Renderer * _renderer;
  };

  // wrapper around texture
  struct SDLTexture
  {
    static shared_ptr< SDLTexture > createSDLTexture(
        const string & filepath,
        SDL_Renderer * renderer);
    ~SDLTexture();
    SDL_Texture * _buffer;
    BoxBoundXYWH _rect;
  private:
    SDLTexture(const string & filepath, SDL_Renderer * renderer);
  };

  // context used by viewport and texture
  struct SDLContext
  {
    SDLContext();

    unique_ptr< SDLRenderer > _view;
    shared_ptr< SDLTexture > _image;
  };

  // graphic functions
  bool initGraphicSystem(Flags flags = 0);
  void quitGraphicSystem();

  // converts the float values from Colour into SDL RGB8
  void colour8RGBA(uint8_t & r, uint8_t & g, uint8_t & b, uint8_t & a, const Engine::Colour & c);

} // end namespace SDLBackEnd

} // end namespace Engine


namespace SDL
{
typedef Engine::GraphicSystemHandler< Engine::SDLBackEnd::SDLContext > GraphicSysHandler;
typedef Engine::ViewPort< Engine::SDLBackEnd::SDLContext > ViewPort;
typedef std::unique_ptr< Engine::ViewPort< Engine::SDLBackEnd::SDLContext > > ViewPortPtr;
typedef Engine::Texture< Engine::SDLBackEnd::SDLContext > Texture;
}

#endif // SDL_CONTEXT_HPP
