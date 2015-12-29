/**
  * helper functions to abstrct the backend
  *
  */

#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include <cstdint>
#include <memory>

#include "data_utils.hpp"

namespace Engine
{
using namespace std;

template < class Impl >
class ViewPort
{
public:
  ~ViewPort(){};

  static shared_ptr<ViewPort> CreateViewPort(BoxBoundary & rect, uint8_t flags = 0);

protected:
  ViewPort(BoxBoundary & rect, uint8_t flags = 0);
  ViewPort() = delete;
  ViewPort(ViewPort & other) = delete;

  //data
  unique_ptr< Impl > _data;
};

template < class Impl >
inline shared_ptr< ViewPort< Impl > >
    ViewPort< Impl >::CreateViewPort(BoxBoundary & rect, uint8_t flags)
{
  return make_shared< ViewPort< Impl > >(rect, flags);
}

} // end namespace Engine

#endif // VIEWPORT_HPP
