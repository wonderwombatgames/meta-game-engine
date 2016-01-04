/**
  * entities manager contains all entities
  * entities are just containers to components
  *
  *
  */

#ifndef GRAPHIC_SYSTEM_HPP
#define GRAPHIC_SYSTEM_HPP

#include <cassert>
#include <string>
#include <unordered_map>
#include <memory>

#include "system_interface.hpp"
#include "command_interface.hpp"
#include "component_graphic.hpp"
#include "graphic_system_handler.hpp"

namespace Engine
{
using namespace std;

//
class GraphicSystem : public ISystem, public ICommand
{
public:
  GraphicSystem(const char * name);
  GraphicSystem() = delete;
  GraphicSystem(GraphicSystem & other) = delete;
  virtual ~GraphicSystem();


protected:
  virtual void onCommand(const CommandMsg & cmd) override {};
  virtual void tick(TimeDimension delta) override {};

  IHandler * _graphicSystemHandler;
};

GraphicSystem::GraphicSystem(const char * name)
    :ISystem(name)
    ,_graphicSystemHandler(nullptr)
{
  //assert(0 == getGraphicHandler(_graphicSystemHandler, nullptr));
}

GraphicSystem::~GraphicSystem()
{
  //delete _graphicSystemHandler;
}

} // end namespace Engine

#endif // GRAPHIC_SYSTEM_HPP
