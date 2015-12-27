/**
  *
  */

#include <cassert>
#include <iostream>
#include "system_interface.hpp"
#include "entities_manager.hpp"

using namespace Engine;

int main()
{
  EntitiesManager * em = EntitiesManager::instance();
  int id1 = em->createEntity<IManagedEntity>("player1");
  assert(em->refreshEntities() == 1);
  cout << "!!!OK => Create Entity" << endl;

  em->destroyEntity(id1);
  assert(em->refreshEntities() == 0);
  cout << "!!!OK => Destroy Entity" << endl;

  int id2 = em->createEntity<IManagedEntity>("player2");
  int id3 = em->createEntity<IManagedEntity>("player3");
  assert(em->refreshEntities() == 2);
  cout << "!!!OK => Create 2 Entities" << endl;

  return 0;
}
