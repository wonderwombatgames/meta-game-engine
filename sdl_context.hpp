/**
  * SDL context to use in the implementation of Texture and Viewport
  * All other contexts should follow more or less the same structure;
  */

#ifndef SDL_CONTEXT_HPP
#define SDL_CONTEXT_HPP

#include <memory>
#include <SDL2/SDL.h>


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
  SDL_Texture * _texture;
};

// context used by viewport and texture
struct SDLContext
{
  SDLContext();

  unique_ptr< SDLRenderer > _view;
  unique_ptr< SDLTexture > _image;
};

} // end namespace Engine

#endif // SDL_CONTEXT_HPP
