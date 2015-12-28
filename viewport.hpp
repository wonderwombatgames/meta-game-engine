/**
  * helper functions to abstrct the backend
  *
  */

#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include <cstdint>
#include "data_utils.hpp"

namespace Engine
{

template< class Impl >
class ViewPort
{
public:
  static ViewPort * CreateViewPort(BoxBoundary & rect, uint8_t flags = 0);

protected:
  Impl _data;
};

} // end namespace Engine

#endif // VIEWPORT_HPP
