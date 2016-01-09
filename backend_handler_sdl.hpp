/**
  * SDL context to use in the implementation of Texture and Viewport
  * All other contexts should follow more or less the same structure;
  */

#ifndef BACKEND_CONTEXT_SDL_HPP
#define BACKEND_CONTEXT_SDL_HPP

#include <memory>
#include <SDL2/SDL.h>
#include "utils_types.hpp"
#include "system_graphics.hpp"
#include "component_graphic_image.hpp"


namespace Engine
{
using namespace std;
using namespace Utils;

namespace BackEnd
{

namespace SDL2
{

  // wrapper around window and renderer
  struct Renderer
  {
    Renderer();
    ~Renderer();
    static Dimension3 _resolution;
    static SDL_Window * _window;
    static SDL_Renderer * _renderer;
  };

  // wrapper around texture
  struct Texture
  {
    static shared_ptr< Texture > createTexture(
        const string & filepath,
        SDL_Renderer * renderer);
    ~Texture();
    SDL_Texture * _buffer;
    BoxBoundXYWH _rect;
  private:
    Texture(const string & filepath, SDL_Renderer * renderer);
  };

  // context used by viewport and texture
  struct Handler
  {
    Handler();

    shared_ptr< Renderer > _view;
    shared_ptr< Texture > _image;
  };

  // graphic functions
  bool initGraphicSystem(Flags flags = 0);
  void quitGraphicSystem();

  // converts the float values from Colour into SDL RGB8
  void colour8RGBA(uint8_t & r, uint8_t & g, uint8_t & b, uint8_t & a, const Engine::Colour & c);

} // end namespace SDL2

} // end namespace BackEnd


namespace SDL2BackEnd
{
typedef Engine::BackEnd::SDL2::Handler Handler;
typedef GraphicDevice::Display< Handler > Display;
typedef GraphicDevice::DisplayInterface DisplayInterface;
typedef std::unique_ptr< DisplayInterface > DisplayPtr;
typedef Component::Image< Handler > Image;
}

} // end namespace Engine

#endif // BACKEND_CONTEXT_SDL_HPP
