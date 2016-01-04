/**
  * helper functions to abstrct the backend
  *
  */

#ifndef COMPONENT_DISPLAY_HPP
#define COMPONENT_DISPLAY_HPP

#include <memory>

#include "component_graphic_texture.hpp"

namespace Engine
{
using namespace std;
using namespace Utils;

namespace Component
{

  template < typename T >
  class Display
  {
  public:
    ~Display(){};
    Display(const BoxBoundXYWH & rect, Flags flags = 0);
    Display() = delete;
    Display(Display & other) = delete;

    // rendering
    void render();
    void clear(const Colour * c = nullptr);

    // reseting the viewport
    void setColour(const Colour & c);
    const Colour & getColour() const;
    void setResolution(Dimension3 & res);
    const Dimension3 & getResolution() const;
    void setViewRect(const BoxBoundXYWH & rect);
    const BoxBoundXYWH & getViewRect() const;
    void setTitle(const string & title);
    const char * getTitle() const;
    void setFullscreen(bool fs);
    const bool isFullscreen() const;

  protected:
    typedef T _HANDLER;

    //data
    unique_ptr< _HANDLER > _data;
    BoxBoundXYWH _rect;
    Colour _background;
  };

} // end namespace Component

} // end namespace Engine

#endif // COMPONENT_DISPLAY_HPP
