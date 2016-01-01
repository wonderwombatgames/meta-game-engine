#include <cassert>
#include "sdl_context.hpp"

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

SDLContext::SDLContext()
    :_view(new SDLRenderer())
    ,_image(nullptr)
{}

} // end namespace Engine
