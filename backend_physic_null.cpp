/**
  * implement physics backend functions = NULL
  *
  */


#include "backend.hpp"

namespace Engine
{
using namespace std;

bool initPhysicSystem(uint flags)
{
  return true;
}

void quitPhysicSystem()
{
}

errorCode getPhysicHandler(IHandler * handler, const IConfig * data)
{
  return 0;
}


} // end namespace Engine
