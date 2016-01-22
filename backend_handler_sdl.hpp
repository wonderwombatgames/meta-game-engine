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

namespace W2E
{
using namespace std;
using namespace Utils;

namespace BE
{

namespace SDL2
{

// wrapper around window and renderer
struct Renderer
{
  Renderer();
  ~Renderer();
  GLOBAL Dimension2 _resolution;
  GLOBAL SDL_Window* _window;
  GLOBAL SDL_Renderer* _renderer;
};

// wrapper around texture
struct Texture
{
  CLASS_METHOD SharedPtr< Texture > createTexture(const String& filepath, SDL_Renderer* renderer);
  ~Texture();
  SDL_Texture* _buffer;
  BoxBoundXYWH _rect;

private:
  Texture(const String& filepath, SDL_Renderer* renderer);
};

// context used by viewport and texture
struct Handler
{
  Handler();

  SharedPtr< Renderer > _view;
  SharedPtr< Texture > _image;
};

// graphic functions
ErrorCode initGraphicSystem(Flags flags = 0);
void quitGraphicSystem();

// converts the float values from Colour into SDL RGB8
void colour8RGBA(u8& r, u8& g, u8& b, u8& a, const Colour& c);

} // end namespace SDL2

} // end namespace BE

namespace SDL2BE
{
using Handler = W2E::BE::SDL2::Handler;
using Image = Component::Image< Handler >;
}

} // end namespace W2E

#endif // BACKEND_CONTEXT_SDL_HPP
