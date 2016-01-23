/**
  *
  */

#include <cstdlib>
#include <chrono>

#include "utils_types.hpp"

namespace W2E
{

namespace Utils
{

// creates a new random ID < MAXIDS_
EntityID rndId()
{
  using namespace std;
  using namespace std::chrono;
  high_resolution_clock::time_point p = high_resolution_clock::now();
  nanoseconds ns = duration_cast< nanoseconds >(p.time_since_epoch());
  time_t t = ns.count();

  u32 rndPrefix = static_cast< u32 >(rand() * 0xff) << 24;
  u32 rId = rndPrefix + (static_cast< u32 >(t) & 0xffffff);
  return rId;
}

EntityID seqId()
{
  LOCALPERSISTENT_ EntityID sId = 0;
  return ++sId;
}

} // end namespace Utils

} // end namespace W2E
