/**
  *
  */

#include <iostream>
#include <cmath>

#include <SDL2/SDL.h>

#include "entities_manager.hpp"
#include "backend_handler_sdl.hpp"
#include "system_graphics.hpp"
#include "system_transform.hpp"

using namespace W2E;
using namespace std;

int main(int argc, char* argv[])
{
  int testcount_ = 0;

  EntitiesManager* em = EntitiesManager::instance();
  cout << "!!!OK - " << ++testcount_ << " => Instantiated Entities Manager. " << endl;

  System::Transform transform;
  cout << "!!!OK - " << ++testcount_ << " => Started Transform System. " << endl;

  System::Graphics graphics;
  cout << "!!!OK - " << ++testcount_ << " => Started Graphics System. " << endl;

  DisplayHandler view = graphics.createDisplay({{{0.0, 0.0}}, {{320.0, 240.0}}});
  cout << "!!!OK - " << ++testcount_ << " => Created a viewport. " << endl;

  EntityID playerId = em->createEntity< EntityBase >("Player");
  cout << "!!!OK - " << ++testcount_ << " => Created a Base entity. " << endl;

  transform.bindComponent(0)->toEntity(em->registrar(playerId));
  cout << "!!!OK - " << ++testcount_ << " => Binded Base entity to a transform component. " << endl;

  ResourceID imgId1 = graphics.loadResourceFromFile< SDL2BE::Image >("img/sample.png");
  cout << "!!!OK - " << ++testcount_ << " => load 1st texture in graphic system " << endl;

  graphics.bindComponent(imgId1)->toEntity(em->registrar(playerId));
  cout << "!!!OK - " << ++testcount_ << " => assigned newly created texture to base entity "
       << endl;

#if 0 // just basic features test

  EntityID id1 = em->createEntity<EntityBase>("player1");
  assert(em->refreshEntities() == 1);
  assert(em->entity(id1).destroy());
  cout << "entities count = " << em->count() << endl;

  assert(em->refreshEntities() == 0);
  cout << "!!!OK - " << ++testcount_ << " => Destroied Entity" << endl;

  EntityID id2 = em->createEntity<EntityBase>("player2");
  EntityID id3 = em->createEntity<EntityBase>("player3");
  assert(em->refreshEntities() == 2);
  cout << "!!!OK - " << ++testcount_ << " => Created 2 Entities" << endl;

  SystemsInterface dummySystem1("dummy1");
  assert(SystemsInterface::isValid(dummySystem1));
  cout << "!!!OK - " << ++testcount_ << " => Created first Dummy System" << endl;

  SystemsInterface dummySystem2("dummy2");
  assert(SystemsInterface::isValid(dummySystem2));
  cout << "!!!OK - " << ++testcount_ << " => Created second Dummy System" << endl;

  assert(em->entity(id2).registerIntoSystem(dummySystem1));
  cout << "!!!OK - " << ++testcount_ << " => Added first Dummy Component to first valid entity" << endl;

  assert(em->entity(id3).registerIntoSystem(dummySystem1));
  cout << "!!!OK - " << ++testcount_ << " => Added first Dummy Component to second valid entity" << endl;

  assert(em->entity(id3).registerIntoSystem(dummySystem2));
  cout << "!!!OK - " << ++testcount_ << " => Added second Dummy Component to second valid entity" << endl;

  id1 = em->createEntity<EntityBase>("player1");
  assert(em->count() == 3);
  cout << "!!!OK - " << ++testcount_ << " => Created third Entity" << endl;

  assert(em->entity(id1).registerIntoSystem(dummySystem2));
  cout << "!!!OK - " << ++testcount_ << " => Added second Dummy Component to third valid entity" << endl;

  for (int i = 0; i < 50; ++i)
  {
    assert(dummySystem1.update(16) == 1+i); // 2 entities * 50 updates
    assert(dummySystem2.update(16) == 1+i); // 2 entities * 50 updates
  }
  cout << "!!!OK - " << ++testcount_ << " => Performed 50 updates on each component system" << endl;

  assert(em->entity(id2).suspend());
  cout << "!!!OK - " << ++testcount_ << " => Suspended 1 entity" << endl;
  assert(!em->entity(id2).suspend());
  cout << "!!!OK - " << ++testcount_ << " => Tried to suspend the same entity" << endl;
  assert(em->entity(id2).resume());
  cout << "!!!OK - " << ++testcount_ << " => Resumed 1 entity" << endl;
  assert(!em->entity(id2).resume());
  cout << "!!!OK - " << ++testcount_ << " => Tried to resume the same entity" << endl;

  assert(em->entity(id1).destroy());
  assert(em->entity(id2).destroy());
  assert(em->entity(id3).destroy());
  assert(em->refreshEntities() == 0);
  cout << "!!!OK - " << ++testcount_ << " => Destroied All Entities" << endl;

#endif

#if 0
  // render by hand
  /////////////////

  // entity pod
  Component::EntityPod entityData_;
  // defaults
  entityData_.entityId = 99;
  // whether or not this entity is active
  entityData_.isActive = true;

  // transform pod
  Component::TransformPod transformData_;
  // kind of space 2D/3D
  transformData_.kind = SPACE2D_;
  // position
  // absolute in pixels (float values)
  // can contain z-order
  transformData_.position = {{ 320.0f, 240.0f, 0.0f }};
  // rotation
  // values between 0.0 - 1.0  (= 0 - 360)
  transformData_.rotation = {{ 0.0f, 0.0f, 0.0f }};
  // scales
  // <1.0 : smaller | > 1.0 : larger
  // <0.0 : mirror
  transformData_.scale = {{ 1.0f, 1.0f, 1.0f }};
  // float rotation = 0.0f;
  // rotation += (1.0f/125.0f);
  // transformData_.rotation.yaw = rotation;

  // image load
  SDL2BE::Image tex1;
  String filename("img/sample.png");
  tex1.loadFromFile(filename);
  cout << "!!!OK - " << ++testcount_ << " => Created 1 texture. " << endl;

  SDL2BE::Image tex2;
  tex2.loadFromFile(filename);
  cout << "!!!OK - " << ++testcount_ << " => Created another texture with same asset. " << endl;

  // tint colour
  Colour c2;
  c2.kind = RGB;
  c2.rgb = {0.1, 0.5, 0.9};

  // graphic pode - Sprite
  Component::GraphicPod sprite;
  sprite.transformData = &transformData_;
  sprite.isVisible = true;
  sprite.anchor = {{ 0.5f, 0.5f, 0.5f }};
  sprite.blendingMode = BLENDMODEALPHA_;
  sprite.alphaMode = 1.0f;
  sprite.colourTint = c2;
  cout << "!!!OK - " << ++testcount_ << " => set the sprite on the world: " << sprite.transformData->position.x << " - " << sprite.transformData->position.y << endl;

#endif

  // setting view
  Dimension2 r{{640.0, 480.0}};
  view->setResolution(r);
  cout << "!!!OK - " << ++testcount_ << " => reset resolution " << endl;
  // view->setFullscreen(true);

  // background colour
  Colour c1;
  c1.kind = RGB;
  c1.rgb = {0.5f, 0.5f, 0.5f};

  view->setColour(c1);
  cout << "!!!OK - " << ++testcount_ << " => reset bg colour " << endl;

  // float randx_ = 0;
  // float randy_ = 0;

  bool running = true;
  SDL_Event event;
  for(int i = 0; running && (i < 250); ++i)
  {
    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_QUIT)
      {
        running = false;
        break;
      }
    }

    // randx_ += -5.0f + ((rand() % 10)+(rand() % 10)+(rand() % 10)) / 3.0f;
    // randy_ += -5.0f + ((rand() % 10)+(rand() % 10)+(rand() % 10)) / 3.0f;
    Component::TransformPod offset{{{-150.0f, -150.0f, 0.0f}}, {{0.0f, 0.0f, 0.0f}}, {{1.0f}}};

    // view->clear(c1);
    // tex2.paint(sprite, offset);
    // view->render();
    graphics.setCameraTransform(offset);
    graphics.update(0.0f);

    SDL_Delay(1000 / 25);
  }
  // SDL_Delay(1000);
  // view->setFullscreen(false);

  return 0;
}
