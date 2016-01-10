/**
  *
  */

#include "utils_types.hpp"

namespace Engine
{

namespace Utils
{

// creates a new random ID < MAX_IDS
EntityID newId()
{
  static std::set<EntityID> container;
  const EntityID maxIds = MAX_IDS;
  // use current time as seed for random generator
  srand(std::time(0));
  EntityID randomId = 1 + ((rand() * (maxIds + 1)) % maxIds);
  int attempts = 0;
  // check that the id was not used yet
  while (container.count(randomId)>0)
  {
    randomId = (rand() * (maxIds + 1)) % maxIds;
    ++attempts;
    // this ensures we do not allocate more entities than the MAX
    assert( attempts < maxIds);
  }
  container.emplace(randomId);
  return randomId;
}


} // end namespace Utils

} // end namespace Engine
