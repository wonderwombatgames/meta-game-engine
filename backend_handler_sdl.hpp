/**
  * SDL context to use in the implementation of Texture and Viewport
  * All other contexts should follow more or less the same structure;
  */

#ifndef BACKEND_CONTEXT_SDL_HPP
#define BACKEND_CONTEXT_SDL_HPP

#include <SDL2/SDL.h>
#include "utils_types.hpp"
#include "utils_colour.hpp"
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
    static Dimension2 _resolution;
    static SDL_Window * _window;
    static SDL_Renderer * _renderer;
  };

  // wrapper around texture
  struct Texture
  {
    static SharedPtr< Texture > createTexture(
        const String & filepath,
        SDL_Renderer * renderer);
    ~Texture();
    SDL_Texture * _buffer;
    BoxBoundXYWH _rect;
  private:
    Texture(const String & filepath, SDL_Renderer * renderer);
  };

  // context used by viewport and texture
  struct Handler
  {
    Handler();

    SharedPtr< Renderer > _view;
    SharedPtr< Texture > _image;
  };

  // graphic functions
  bool initGraphicSystem(Flags flags = 0);
  void quitGraphicSystem();

  // converts the float values from Colour into SDL RGB8
  void colour8RGBA(u8 & r, u8 & g, u8 & b, u8 & a, const Colour & c);

} // end namespace SDL2

} // end namespace BackEnd


namespace SDL2BackEnd
{
using Handler = Engine::BackEnd::SDL2::Handler;
using Display = GraphicDevice::Display< Handler >;
using DisplayInterface = GraphicDevice::DisplayInterface;
using DisplayPtr = UniquePtr< DisplayInterface >;
using Image = Component::Image< Handler >;
}

} // end namespace Engine

#endif // BACKEND_CONTEXT_SDL_HPP
