/**
  * wraps functionality of the graphic backend to be used by the system
  *
  */

#ifndef GRAPHIC_SYSTEM_HANDLER_HPP
#define GRAPHIC_SYSTEM_HANDLER_HPP

#include "backend.hpp"
#include "viewport.hpp"
#include "texture.hpp"

namespace Engine
{
using namespace std;

template< class Context >
class GraphicSystemHandler : public IHandler
{
public:
  GraphicSystemHandler();
  virtual ~GraphicSystemHandler();

  ErrorCode startViewPort(ViewPort< Context > * view, BoxBoundary & rect, Flags flags = 0);
  ErrorCode loadTexture(Texture< Context > * tex, const char * filepath, const char * atlas = nullptr);
};

}// end namespace Engine

#endif // GRAPHIC_SYSTEM_HANDLER_HPP
