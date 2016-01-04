/**
  * wraps functionality of the graphic backend to be used by the system
  *
  */

#ifndef GRAPHIC_SYSTEM_HANDLER_HPP
#define GRAPHIC_SYSTEM_HANDLER_HPP

#include <memory>

#include "backend.hpp"
#include "component_graphic_viewport.hpp"
#include "component_graphic_texture.hpp"

namespace Engine
{
using namespace std;
using namespace BackEnd;

template< typename T >
class GraphicSystemHandler : public IHandler
{
public:
  GraphicSystemHandler();
  virtual ~GraphicSystemHandler();

  std::unique_ptr< Component::Display< T > > getDisplay();
  // ErrorCode startDisplay(Display< T > * view, BoxBound & rect, Flags flags = 0);
  // ErrorCode loadTexture(Texture< T > * tex, const char * filepath, const char * atlas = nullptr);
};

}// end namespace Engine

#endif // GRAPHIC_SYSTEM_HANDLER_HPP
