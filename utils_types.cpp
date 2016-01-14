/**
  *
  */

#include <cstdlib>
#include <chrono>

#include "utils_types.hpp"


namespace Engine
{

namespace Utils
{

// creates a new random ID < MAX_IDS
EntityID newId()
{

  using namespace std::chrono;
  high_resolution_clock::time_point p = high_resolution_clock::now();
  nanoseconds ns = duration_cast<nanoseconds>(p.time_since_epoch());
  std::time_t t = ns.count();

  uint32_t rndPrefix = static_cast<uint32_t>(rand() * 0xff) << 24;
  uint32_t randomId = rndPrefix + (static_cast<uint32_t>(t) & 0xffffff );
  return randomId;
}


} // end namespace Utils

} // end namespace Engine
