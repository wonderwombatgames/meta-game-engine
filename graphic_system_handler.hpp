/**
  * wraps functionality of the graphic backend to be used by the system
  *
  */

#ifndef GRAPHIC_SYSTEM_HANDLER_HPP
#define GRAPHIC_SYSTEM_HANDLER_HPP

#include "backend.hpp"
#include "data_utils.hpp"

namespace Engine
{
using namespace std;

class GraphicSystemHandler : public IHandler
{
public:
  virtual ~GraphicSystemHandler(){}

  template< class Texture >
  errorCode LoadTexture(Texture * tex, const char * filepath, const char * atlas = nullptr);
  template< class ViewPort >
  errorCode CreateViewPort(ViewPort * view, BoxBoundary & rect, uint8_t flags = 0);
};

}// end namespace Engine

#endif // GRAPHIC_SYSTEM_HANDLER_HPP
