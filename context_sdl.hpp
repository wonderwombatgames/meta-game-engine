/**
  * SDL context to use in the implementation of Texture and Viewport
  * All other contexts should follow more or less the same structure;
  */

#ifndef SDL_CONTEXT_HPP
#define SDL_CONTEXT_HPP

#include <memory>
#include <SDL2/SDL.h>
#include "basic_types.hpp"


namespace Engine
{
using namespace std;

  // wrapper around window and renderer
  struct SDLRenderer
  {
    SDLRenderer();
    ~SDLRenderer();
    static SDL_Window * _window;
    static SDL_Renderer * _renderer;
  };

  // wrapper around texture
  struct SDLTexture
  {
    SDLTexture(const string & filepath, SDL_Renderer * renderer);
    ~SDLTexture();
    SDL_Texture * _buffer;
  };

  // context used by viewport and texture
  struct SDLContext
  {
    SDLContext();

    unique_ptr< SDLRenderer > _view;
    unique_ptr< SDLTexture > _image;
  };

namespace SDLBackEnd
{

  // graphic functions
  bool initGraphicSystem(Flags flags = 0);
  void quitGraphicSystem();

  // converts the float values from Colour into SDL RGB8
  void colour8RGBA(uint8_t & r, uint8_t & g, uint8_t & b, uint8_t & a, const Engine::Colour & c);

} // end namespace SDLBackEnd

} // end namespace Engine

#endif // SDL_CONTEXT_HPP
