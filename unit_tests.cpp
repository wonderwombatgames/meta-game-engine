/**
  *
  */

#include <iostream>
#include <cmath>

#include <SDL2/SDL.h>

#include "entities_manager.hpp"
#include "system_interface.hpp"
#include "backend_handler_sdl.hpp"
#include "system_graphics.hpp"


using namespace Engine;

int main(int argc, char *argv[])
{
  int test_count = 0;

  EntitiesManager * em = EntitiesManager::instance();
  int id1 = em->createEntity<BaseEntity>("player1");
  assert(em->refreshEntities() == 1);
  assert(em->destroyEntity(id1));
  cout << "entities count = " << em->count() << endl;

  assert(em->refreshEntities() == 0);
  cout << "!!!OK - " << ++test_count << " => Destroied Entity" << endl;

  int id2 = em->createEntity<BaseEntity>("player2");
  int id3 = em->createEntity<BaseEntity>("player3");
  assert(em->refreshEntities() == 2);
  cout << "!!!OK - " << ++test_count << " => Created 2 Entities" << endl;

  SystemsInterface dummySystem1("dummy1");
  assert(SystemsInterface::isValid(dummySystem1));
  cout << "!!!OK - " << ++test_count << " => Created first Dummy System" << endl;

  SystemsInterface dummySystem2("dummy2");
  assert(SystemsInterface::isValid(dummySystem2));
  cout << "!!!OK - " << ++test_count << " => Created second Dummy System" << endl;

  assert(em->addComponent(id2, dummySystem1));
  cout << "!!!OK - " << ++test_count << " => Added first Dummy Component to first valid entity" << endl;

  assert(em->addComponent(id3, dummySystem1));
  cout << "!!!OK - " << ++test_count << " => Added first Dummy Component to second valid entity" << endl;

  assert(em->addComponent(id3, dummySystem2));
  cout << "!!!OK - " << ++test_count << " => Added second Dummy Component to second valid entity" << endl;

  id1 = em->createEntity<BaseEntity>("player1");
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

  System::Graphics graphics("window1");
  graphics.createDisplay({{0.0,0.0,0.0}, {320.0, 240.0, 0.0}});
  SDL2BackEnd::DisplayInterface & view = *graphics.display.get();
  //     *(graphics.createDisplay<SDL2BackEnd::Display>
  //         ({{0.0,0.0,0.0}, {320.0, 240.0, 0.0}}));
  // SDL2BackEnd::Display view({{0.0,0.0,0.0}, {320.0, 240.0, 0.0}});
  cout << "!!!OK - " << ++test_count << " => Created a viewport. " << endl;

  Component::EntityPod _entityData;
  // defaults
  _entityData.entityId = 0;
  // whether or not this entity is active
  _entityData.isActive = true;

  Component::TransformPod _transformData;
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

  SDL2BackEnd::Image tex1;
  string filename("img/sample.png");
  tex1.loadFromFile(filename);
  cout << "!!!OK - " << ++test_count << " => Created 1 texture. " << endl;

  SDL2BackEnd::Image tex2;
  tex2.loadFromFile(filename);
  cout << "!!!OK - " << ++test_count << " => Created another texture with same asset. " << endl;

  Component::GraphicPod sprite;
  sprite.transformData = &_transformData;
  sprite.isVisible = true;
  sprite.anchor = {0.5f, 0.5f, 0.5f};
  sprite.blendingMode = SDL_BLENDMODE_BLEND;
  sprite.alphaMode = 1.0f;
  Colour c2;
  c2.kind = RGB;
  c2.rgb = {0.1, 0.5, 0.9};
  sprite.colourTint = c2;
  cout << "!!!OK - " << ++test_count << " => set the sprite on the world: " << sprite.transformData->position.x << " - " << sprite.transformData->position.y << endl;


  Dimension3 r{640.0, 480.0, 0.0};
  view.setResolution(r);
  cout << "!!!OK - " << ++test_count << " => reset resolution " << endl;
  //view.setFullscreen(true);

  Colour c1;
  c1.kind = RGB;
  c1.rgb = {0.5, 0.5, 0.5};

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

    view.clear(c1);
    tex2.paint(sprite, offset);
    view.render();

    SDL_Delay(1000/25);
  }
  cout << "!!!OK - " << ++test_count << " => Painting texture. " << endl;
  // SDL_Delay(1000);
  // view.setFullscreen(false);

  return 0;
}
