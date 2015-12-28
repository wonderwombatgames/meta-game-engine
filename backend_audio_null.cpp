/**
  * implement audio backend functions = NULL
  *
  */


#include "backend.hpp"

namespace Engine
{
using namespace std;

bool initAudioSystem(uint flags)
{
  return true;
}

void quitAudioSystem()
{
}

errorCode getAudioHandler(IHandler * handler, const IConfig * data)
{
  return 0;
}

} // end namespace Engine
