/**
  *
  */

#include <iostream>
#include <cmath>

#include <SDL2/SDL.h>

#include "entities_manager.hpp"
#include "entity_transformable.hpp"
#include "system_interface.hpp"
#include "backend_handler_sdl.hpp"
#include "system_graphics.hpp"
#include "system_transform.hpp"


using namespace W2E;

int main(int argc, char *argv[])
{
  int test_count = 0;

  EntitiesManager * em = EntitiesManager::instance();
  cout << "!!!OK - " << ++test_count << " => Instantiated Entities Manager. " << endl;

  System::Transform transform;
  cout << "!!!OK - " << ++test_count << " => Started Transform System. " << endl;

  System::Graphics graphics;
  cout << "!!!OK - " << ++test_count << " => Started Graphics System. " << endl;

  DisplayHandler view = graphics.createDisplay({ {{0.0, 0.0}}, {{320.0, 240.0}} });
  cout << "!!!OK - " << ++test_count << " => Created a viewport. " << endl;

  EntityID entId = em->createEntity<EntityTransformable>("Player");
  cout << "!!!OK - " << ++test_count << " => Created a Trasnformable entity. " << endl;

  ResourceID imgId = graphics.loadResourceFromFile<SDL2BE::Image>("img/sample.png");
  cout << "!!!OK - " << ++test_count << " => load texture in graphic system " << endl;

  // graphics.setEntityAsset(entId, imgId);
  graphics.bindResource(imgId)->toEntity(&(em->entity(entId)));
  cout << "!!!OK - " << ++test_count << " => assigned newly created texture to transformable entity " << endl;


#if 0 // just basic features test

  EntityID id1 = em->createEntity<EntityBase>("player1");
  assert(em->refreshEntities() == 1);
  assert(em->entity(id1).destroy());
  cout << "entities count = " << em->count() << endl;

  assert(em->refreshEntities() == 0);
  cout << "!!!OK - " << ++test_count << " => Destroied Entity" << endl;

  EntityID id2 = em->createEntity<EntityBase>("player2");
  EntityID id3 = em->createEntity<EntityBase>("player3");
  assert(em->refreshEntities() == 2);
  cout << "!!!OK - " << ++test_count << " => Created 2 Entities" << endl;

  SystemsInterface dummySystem1("dummy1");
  assert(SystemsInterface::isValid(dummySystem1));
  cout << "!!!OK - " << ++test_count << " => Created first Dummy System" << endl;

  SystemsInterface dummySystem2("dummy2");
  assert(SystemsInterface::isValid(dummySystem2));
  cout << "!!!OK - " << ++test_count << " => Created second Dummy System" << endl;

  assert(em->entity(id2).registerIntoSystem(dummySystem1));
  cout << "!!!OK - " << ++test_count << " => Added first Dummy Component to first valid entity" << endl;

  assert(em->entity(id3).registerIntoSystem(dummySystem1));
  cout << "!!!OK - " << ++test_count << " => Added first Dummy Component to second valid entity" << endl;

  assert(em->entity(id3).registerIntoSystem(dummySystem2));
  cout << "!!!OK - " << ++test_count << " => Added second Dummy Component to second valid entity" << endl;

  id1 = em->createEntity<EntityBase>("player1");
  assert(em->count() == 3);
  cout << "!!!OK - " << ++test_count << " => Created third Entity" << endl;

  assert(em->entity(id1).registerIntoSystem(dummySystem2));
  cout << "!!!OK - " << ++test_count << " => Added second Dummy Component to third valid entity" << endl;

  for (int i = 0; i < 50; ++i)
  {
    assert(dummySystem1.update(16) == 1+i); // 2 entities * 50 updates
    assert(dummySystem2.update(16) == 1+i); // 2 entities * 50 updates
  }
  cout << "!!!OK - " << ++test_count << " => Performed 50 updates on each component system" << endl;

  assert(em->entity(id2).suspend());
  cout << "!!!OK - " << ++test_count << " => Suspended 1 entity" << endl;
  assert(!em->entity(id2).suspend());
  cout << "!!!OK - " << ++test_count << " => Tried to suspend the same entity" << endl;
  assert(em->entity(id2).resume());
  cout << "!!!OK - " << ++test_count << " => Resumed 1 entity" << endl;
  assert(!em->entity(id2).resume());
  cout << "!!!OK - " << ++test_count << " => Tried to resume the same entity" << endl;

  assert(em->entity(id1).destroy());
  assert(em->entity(id2).destroy());
  assert(em->entity(id3).destroy());
  assert(em->refreshEntities() == 0);
  cout << "!!!OK - " << ++test_count << " => Destroied All Entities" << endl;

#endif

  // render by hand
  /////////////////

  // entity pod
  Component::EntityPod _entityData;
  // defaults
  _entityData.entityId = 99;
  // whether or not this entity is active
  _entityData.isActive = true;

  // transform pod
  Component::TransformPod _transformData;
  // kind of space 2D/3D
  _transformData.kind = SPACE_2D;
  // position
  // absolute in pixels (float values)
  // can contain z-order
  _transformData.position = {{ 320.0f, 240.0f, 0.0f }};
  // rotation
  // values between 0.0 - 1.0  (= 0 - 360)
  _transformData.rotation = {{ 0.0f, 0.0f, 0.0f }};
  // scales
  // <1.0 : smaller | > 1.0 : larger
  // <0.0 : mirror
  _transformData.scale = {{ 1.0f, 1.0f, 1.0f }};
  // float rotation = 0.0f;
  // rotation += (1.0f/125.0f);
  // _transformData.rotation.yaw = rotation;

  // image load
  SDL2BE::Image tex1;
  String filename("img/sample.png");
  tex1.loadFromFile(filename);
  cout << "!!!OK - " << ++test_count << " => Created 1 texture. " << endl;

  SDL2BE::Image tex2;
  tex2.loadFromFile(filename);
  cout << "!!!OK - " << ++test_count << " => Created another texture with same asset. " << endl;

  // tint colour
  Colour c2;
  c2.kind = RGB;
  c2.rgb = {0.1, 0.5, 0.9};

  // graphic pode - Sprite
  Component::GraphicPod sprite;
  sprite.transformData = &_transformData;
  sprite.isVisible = true;
  sprite.anchor = {{ 0.5f, 0.5f, 0.5f }};
  sprite.blendingMode = SDL_BLENDMODE_BLEND;
  sprite.alphaMode = 1.0f;
  sprite.colourTint = c2;
  cout << "!!!OK - " << ++test_count << " => set the sprite on the world: " << sprite.transformData->position.x << " - " << sprite.transformData->position.y << endl;

  // setting view
  Dimension2 r{{ 640.0, 480.0 }};
  view->setResolution(r);
  cout << "!!!OK - " << ++test_count << " => reset resolution " << endl;
  //view->setFullscreen(true);

  // background colour
  Colour c1;
  c1.kind = RGB;
  c1.rgb = {0.5f, 0.5f, 0.5f};

  // float rand_x = 0;
  // float rand_y = 0;

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

    // rand_x += -5.0f + ((rand() % 10)+(rand() % 10)+(rand() % 10)) / 3.0f;
    // rand_y += -5.0f + ((rand() % 10)+(rand() % 10)+(rand() % 10)) / 3.0f;
    Component::TransformPod offset{
        {{-150.0f, -150.0f, 0.0f}},
        {{0.0f, 0.0f, 0.0f}},
        {{1.0f}} };

    view->clear(c1);
    // tex2.paint(sprite, offset);
    graphics.setCameraTransform(offset);
    graphics.update(0.0f);
    view->render();

    SDL_Delay(1000/25);
  }
  // SDL_Delay(1000);
  // view->setFullscreen(false);

  return 0;
}
