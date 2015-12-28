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
#include "data_utils.hpp"
#include "backend.hpp"

// forward declaration
class TextureAtlas;

//class GraphicEffectsData;
//class ShaderData;
//class ParticlesEmitterData;

namespace Engine
{
using namespace std;

struct GraphicsAndAnimationsComponent
{
  // defines the anchor within the boudaries
  // and rendering = drawing pivot
  // values between 0.0 - 1.0 (in relation to entity size | UV)
  Vector3 anchor;

  // colour parameters
  Colour colourMode;
  uint8_t alphaMode;
  uint8_t blendingMode;

  // texture data
  TextureAtlas * texture;
  unsigned int animationFrame;

  // special components
  //GraphicEffectsData * effects;
  //ShaderData * shader;
  //ParticlesEmitterData * emitter;

};


//
class GraphicSystem : public ISystem
{
public:
  virtual ~GraphicSystem();
  GraphicSystem(const char * name);
  GraphicSystem() = delete;
  GraphicSystem(GraphicSystem & other) = delete;

protected:
  virtual void run(IManagedEntity * entity) override {};
};

GraphicSystem::~GraphicSystem()
{
}
GraphicSystem::GraphicSystem(const char * name): ISystem(name)
{
  assert(initGraphicSystem(0));
}

} // end namespace Engine

#endif // GRAPHIC_SYSTEM_HPP
