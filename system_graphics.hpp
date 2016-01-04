/**
  * entities manager contains all entities
  * entities are just containers to components
  *
  *
  */

#ifndef GRAPHIC_SYSTEMS_HPP
#define GRAPHIC_SYSTEMS_HPP

#include <cassert>
#include <string>
#include <unordered_map>
#include <memory>

#include "system_interface.hpp"
#include "command_interface.hpp"
#include "component_graphic.hpp"

namespace Engine
{
using namespace std;

namespace System
{

  class Graphics : public BaseInterface, public ICommand
  {
  public:
    Graphics(const char * name);
    Graphics() = delete;
    Graphics(Graphics & other) = delete;
    virtual ~Graphics();


  protected:
    virtual void onCommand(const CommandMsg & cmd) override {};
    virtual void tick(TimeDimension delta) override {};

    //IHandler * _graphicSystemHandler;
  };


} // end namespace System

} // end namespace Engine

#endif // GRAPHIC_SYSTEMS_HPP
