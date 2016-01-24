/**
  * implement input backend functions using SDL
  *
  */

#include <cassert>

#include "backend_handler_sdl.hpp"
#include "system_input.hpp"

namespace W2E
{

namespace Component
{
using namespace BE;

void WasdInput::poll(const Event& event, InputPod& component)
{
  if(event.type == SDL_KEYDOWN)
  {
    // update axis based on key press
    switch(event.value)
    {
    case SDLK_w:
      component.axis[0] = -1.0f;
      break;

    case SDLK_s:
      component.axis[0] = 1.0f;
      break;

    case SDLK_a:
      component.axis[1] = -1.0f;
      break;

    case SDLK_d:
      component.axis[1] = -1.0f;
      break;

      // TODO: missing buttons... Another component???
    }
  }
}

void ArrowsInput::poll(const Event& event, InputPod& component)
{
  if(event.type == SDL_KEYDOWN)
  {
    // update axis based on key press
    switch(event.value)
    {
    case SDLK_UP:
      component.axis[0] = -1.0f;
      break;

    case SDLK_DOWN:
      component.axis[0] = 1.0f;
      break;

    case SDLK_LEFT:
      component.axis[1] = -1.0f;
      break;

    case SDLK_RIGHT:
      component.axis[1] = -1.0f;
      break;

      // TODO: missing buttons... Another component???
    }
  }
}

// void XInput::poll(const Event& event, InputPod& component)
//{
//
//}
//
// void NumPadInput::poll(const Event& event, InputPod& component)
//{
//
//}

} // end namespace Component

namespace System
{

using namespace BE;

Input::Input()
    : SystemsInterface{"Input"}
    , components_{}
    , resources_{}
{
  SDL2::initInputSystem();
}

Input::Input(const char* name)
    : SystemsInterface{name}
    , components_{}
    , resources_{}
{
  SDL2::initInputSystem();
}

Input::~Input() { SDL2::quitInputSystem(); }

void Input::tick(TimeDim delta)
{
  SDL_Event event;
  while(SDL_PollEvent(&event))
  {
    if(event.type == SDL_QUIT)
    {
      exit(0);
    }
    for(auto comp : this->components_)
    {
      if(comp.second.resource)
      {
        Component::Event e_;
        e_.type = static_cast< u16 >(event.type);
        // FIXME: this only works for keyboard!!!
        e_.value = static_cast< u16 >(event.key.keysym.sym);
        comp.second.resource->poll(e_, comp.second);
      }
    }
  }
}

} // end namespace System

} // end namespace W2E
