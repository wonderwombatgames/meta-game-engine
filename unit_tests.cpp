/**
  *
  */

#include <iostream>
#include "system_interface.hpp"
#include "entities_manager.hpp"

using namespace Engine;

int main()
{
  int test_count = 0;

  EntitiesManager * em = EntitiesManager::instance();
  int id1 = em->createEntity<IManagedEntity>("player1");
  assert(em->refreshEntities() == 1);
  cout << "!!!OK - " << ++test_count << " => Created Entity" << endl;

  assert(em->destroyEntity(id1));
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
  assert(em->size() == 3);
  cout << "!!!OK - " << ++test_count << " => Created third Entity" << endl;

  assert(em->addComponent(id1, dummySystem2));
  cout << "!!!OK - " << ++test_count << " => Added second Dummy Component to third valid entity" << endl;

  for (int i = 0; i < 50; ++i)
  {
    em->updateComponents(dummySystem1);
    em->updateComponents(dummySystem2);
  }
  assert(dummySystem1.getFrame() == 100); // 2 entities * 50 updates
  assert(dummySystem2.getFrame() == 100); // 2 entities * 50 updates
  cout << "!!!OK - " << ++test_count << " => Performed 50 updates on each component system" << endl;

  for (int i = 0; i < 50; ++i)
  {
    em->updateComponents(dummySystem1);
    em->updateComponents(dummySystem2);
    if(i == 24)
    {
      assert(em->suspendEntity(id1));
    }
    if(i == 34)
    {
      assert(em->resumeEntity(id1));
    }
  }
  assert(dummySystem1.getFrame() == 200); // 100 + 2 entities * 50 updates
  assert(dummySystem2.getFrame() == 190); // 100 + 1 entities * 50 updates + 1 entity * 40
  // cout << "dummy1 frames: " << dummySystem1.getFrame() << endl;
  // cout << "dummy2 frames: " << dummySystem2.getFrame() << endl;
  cout << "!!!OK - " << ++test_count << " => Performed 50 more updates per system - suspended 1 entity during 10 frames" << endl;

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

  return 0;
}
