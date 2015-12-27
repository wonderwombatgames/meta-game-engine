/**
  *
  */

#include <iostream>
#include "system_interface.hpp"
#include "entities_manager.hpp"

using namespace Engine;

int main()
{
  EntitiesManager * em = EntitiesManager::instance();
  int id1 = em->createEntity<IManagedEntity>("player1");
  assert(em->refreshEntities() == 1);
  cout << "!!!OK => Created Entity" << endl;

  assert(em->destroyEntity(id1));
  assert(em->refreshEntities() == 0);
  cout << "!!!OK => Destroied Entity" << endl;

  int id2 = em->createEntity<IManagedEntity>("player2");
  int id3 = em->createEntity<IManagedEntity>("player3");
  assert(em->refreshEntities() == 2);
  cout << "!!!OK => Created 2 Entities" << endl;

  ISystem dummySystem1("dummy1");
  assert(ISystem::isValid(&dummySystem1));
  cout << "!!!OK => Created first Dummy System" << endl;

  ISystem dummySystem2("dummy2");
  assert(ISystem::isValid(&dummySystem2));
  cout << "!!!OK => Created second Dummy System" << endl;

  assert(em->addComponent(id2, &dummySystem1));
  cout << "!!!OK => Added first Dummy Component to first valid entity" << endl;

  assert(em->addComponent(id3, dummySystem1));
  cout << "!!!OK => Added first Dummy Component to second valid entity" << endl;

  assert(em->addComponent(id3, &dummySystem2));
  cout << "!!!OK => Added second Dummy Component to second valid entity" << endl;

  id1 = em->createEntity<IManagedEntity>("player1");
  assert(em->size() == 3);
  cout << "!!!OK => Created third Entity" << endl;

  assert(em->addComponent(id1, &dummySystem2));
  cout << "!!!OK => Added second Dummy Component to third valid entity" << endl;

  for (int i = 0; i < 50; ++i)
  {
    em->updateComponents(dummySystem1);
    em->updateComponents(&dummySystem2);
  }
  // cout << "dummy1 frames: " << dummySystem1.getFrame() << endl;
  // cout << "dummy2 frames: " << dummySystem2.getFrame() << endl;
  assert(dummySystem1.getFrame() == 100); // 2 entities * 50 updates
  assert(dummySystem2.getFrame() == 100); // 2 entities * 50 updates
  cout << "!!!OK => Performed 50 updates on each component system" << endl;

  assert(em->destroyEntity(id1));
  assert(em->destroyEntity(id2));
  assert(em->destroyEntity(id3));
  assert(em->refreshEntities() == 0);
  cout << "!!!OK => Destroied All Entities" << endl;

  return 0;
}
