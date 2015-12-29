/**
  * wraps functionality of the graphic backend to be used by the system
  *
  */

#ifndef GRAPHIC_SYSTEM_HANDLER_HPP
#define GRAPHIC_SYSTEM_HANDLER_HPP

#include "backend.hpp"
#include "data_utils.hpp"
#include "viewport.hpp"
#include "texture.hpp"

namespace Engine
{
using namespace std;

template< class Impl >
class GraphicSystemHandler : public IHandler
{
public:
  virtual ~GraphicSystemHandler(){}

  ErrorCode LoadTexture(Texture * tex, const char * filepath, const char * atlas = nullptr);

  ErrorCode CreateViewPort(ViewPort< Impl > * view, BoxBoundary & rect, Flags flags = 0);
};

}// end namespace Engine

#endif // GRAPHIC_SYSTEM_HANDLER_HPP
