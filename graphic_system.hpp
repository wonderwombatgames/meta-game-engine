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
#include "graphic_component.hpp"
#include "graphic_system_handler.hpp"

namespace Engine
{
using namespace std;

//
class GraphicSystem : public ISystem
{
public:
  GraphicSystem(const char * name);
  GraphicSystem() = delete;
  GraphicSystem(GraphicSystem & other) = delete;
  virtual ~GraphicSystem();

protected:
  virtual void tick(TimeDimension delta) override {};
  IHandler * _graphicSystemHandler;
};

GraphicSystem::GraphicSystem(const char * name)
    :ISystem(name)
    ,_graphicSystemHandler(nullptr)
{
  assert(initGraphicSystem(0));
}

GraphicSystem::~GraphicSystem()
{
  quitGraphicSystem();
}

} // end namespace Engine

#endif // GRAPHIC_SYSTEM_HPP
