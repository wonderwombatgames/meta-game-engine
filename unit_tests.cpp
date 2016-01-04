/**
  *
  */

#include <iostream>
#include <cmath>

#include <SDL2/SDL.h>

#include "entities_manager.hpp"
#include "system_interface.hpp"
#include "backend.hpp"
// #include "viewport.hpp"
#include "backend_context_sdl.hpp"
#include "graphic_system_handler.hpp"


using namespace Engine;

int main(int argc, char *argv[])
{
  int test_count = 0;

  EntitiesManager * em = EntitiesManager::instance();
  int id1 = em->createEntity<IManagedEntity>("player1");
  assert(em->refreshEntities() == 1);
  assert(em->destroyEntity(id1));
  cout << "entities count = " << em->count() << endl;

  assert(em->refreshEntities() == 0);
  cout << "!!!OK - " << ++test_count << " => Destroied Entity" << endl;

  int id2 = em->createEntity<IManagedEntity>("player2");
  int id3 = em->createEntity<IManagedEntity>("player3");
  assert(em->refreshEntities() == 2);
  cout << "!!!OK - " << ++test_count << " => Created 2 Entities" << endl;

  ISystem dummySystem1("dummy1");
  assert(ISystem::isValid(dummySystem1));
  cout << "!!!OK - " << ++test_count << " => Created first Dummy System" << endl;

  ISystem dummySystem2("dummy2");
  assert(ISystem::isValid(dummySystem2));
  cout << "!!!OK - " << ++test_count << " => Created second Dummy System" << endl;

  assert(em->addComponent(id2, dummySystem1));
  cout << "!!!OK - " << ++test_count << " => Added first Dummy Component to first valid entity" << endl;

  assert(em->addComponent(id3, dummySystem1));
  cout << "!!!OK - " << ++test_count << " => Added first Dummy Component to second valid entity" << endl;

  assert(em->addComponent(id3, dummySystem2));
  cout << "!!!OK - " << ++test_count << " => Added second Dummy Component to second valid entity" << endl;

  id1 = em->createEntity<IManagedEntity>("player1");
  assert(em->count() == 3);
  cout << "!!!OK - " << ++test_count << " => Created third Entity" << endl;

  assert(em->addComponent(id1, dummySystem2));
  cout << "!!!OK - " << ++test_count << " => Added second Dummy Component to third valid entity" << endl;

  for (int i = 0; i < 50; ++i)
  {
    assert(dummySystem1.update(16) == 1+i); // 2 entities * 50 updates
    assert(dummySystem2.update(16) == 1+i); // 2 entities * 50 updates
  }
  cout << "!!!OK - " << ++test_count << " => Performed 50 updates on each component system" << endl;

  assert(em->suspendEntity(id2));
  cout << "!!!OK - " << ++test_count << " => Suspended 1 entity" << endl;
  assert(!em->suspendEntity(id2));
  cout << "!!!OK - " << ++test_count << " => Tried to suspend the same entity" << endl;
  assert(em->resumeEntity(id2));
  cout << "!!!OK - " << ++test_count << " => Resumed 1 entity" << endl;
  assert(!em->resumeEntity(id2));
  cout << "!!!OK - " << ++test_count << " => Tried to resume the same entity" << endl;

  assert(em->destroyEntity(id1));
  assert(em->destroyEntity(id2));
  assert(em->destroyEntity(id3));
  assert(em->refreshEntities() == 0);
  cout << "!!!OK - " << ++test_count << " => Destroied All Entities" << endl;

  //GraphicSystemHandler< Engine::SDLBackEnd::SDLContext > graphics;
  //SDLBackEnd::initGraphicSystem();
  //BoxBoundXYWH bb{{0.0,0.0,0.0}, {320.0, 240.0, 0.0}};
  //ViewPort< Engine::SDLBackEnd::SDLContext > view(bb);

  SDL2BackEnd::GraphicSysHandler graphics;
  SDL2BackEnd::ViewPortPtr view = std::move(graphics.getViewPort());
  //SDL::ViewPort view({{0.0,0.0,0.0}, {320.0, 240.0, 0.0}});
  cout << "!!!OK - " << ++test_count << " => Created a viewport. " << endl;

  EntityComponent _entityData;
  // defaults
  _entityData.entityId = 0;
  // whether or not this entity is active
  _entityData.isActive = true;

  TransformComponent _transformData;
  // kind of space 2D/3D
  _transformData.kind = SPACE_2D;
  // position
  // absolute in pixels (float values)
  // can contain z-order
  _transformData.position = {320.0f, 240.0f, 0.0f};
  // rotation
  // values between 0.0 - 1.0  (= 0 - 360)
  _transformData.rotation = {0.0f, 0.0f, 0.0f};
  // scales
  // <1.0 : smaller | > 1.0 : larger
  // <0.0 : mirror
  _transformData.scale = {1.0f, 1.0f, 1.0f};

  GraphicComponent sprite_;
  //Texture< Engine::SDLBackEnd::SDLContext > tex1(sprite_);
  SDL2BackEnd::Texture tex1(sprite_);
  string filename("img/sample.png");
  tex1.loadFromFile(filename);

  GraphicComponent sprite;
  //Texture< Engine::SDLBackEnd::SDLContext > tex2(sprite);
  SDL2BackEnd::Texture tex2(sprite);
  tex2.loadFromFile(filename);

  Colour c1;
  c1.kind = RGB;
  c1.rgb = {0.5, 0.5, 0.5};
  Dimension3 r{640.0, 480.0, 0.0};

  sprite.transformData = &_transformData;
  sprite.anchor = {0.5f, 0.5f, 0.5f};
  sprite.blendingMode = SDL_BLENDMODE_BLEND;
  sprite.alphaMode = 1.0f;
  Colour c2;
  c2.kind = RGB;
  c2.rgb = {0.1, 0.5, 0.9};
  //sprite.colourTint = c2;

  view->setResolution(r);
  //view.setFullscreen(true);

  float rotation = 0.0f;
  float rand_x = 0;
  float rand_y = 0;
  bool running = true;
  SDL_Event event;
  for (int i = 0; running && (i < 250); ++i)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
          running = false;
          break;
      }
    }
    rotation += (1.0f/125.0f);
    _transformData.rotation = {rotation, 0.0f, 0.0f};
    rand_x += -5.0f + ((rand() % 10)+(rand() % 10)+(rand() % 10)) / 3.0f;
    rand_y += -5.0f + ((rand() % 10)+(rand() % 10)+(rand() % 10)) / 3.0f;
    Vector3 offset{rand_x, rand_y, 0.0f};

    // view.setColour(c1);
    view->clear(&c1);
    tex2.paint(offset);
    view->render();

    SDL_Delay(1000/25);
  }
  cout << "!!!OK - " << ++test_count << " => Painting texture. " << endl;
  // SDL_Delay(1000);
  //view.setFullscreen(false);

  //SDLBackEnd::quitGraphicSystem();

  return 0;
}
