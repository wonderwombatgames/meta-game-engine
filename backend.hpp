/**
  * helper functions to abstrct the backend
  *
  */

#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <memory>

namespace Engine
{
using namespace std;

typedef uint8_t errorCode;

class IHandler
{
  virtual ~IHandler(){}
};

class IConfig
{
  virtual ~IConfig(){}
};


// graphics functions
bool initGraphicSystem(uint flags);
void quitGraphicSystem();
errorCode getGraphicHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);

// inputs functions
bool initInputSystem(uint flags);
void quitInputSystem();
errorCode getInputHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);

// events functions
bool initEventSystem(uint flags);
void quitEventSystem();
errorCode getEventHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);

// physics functions
bool initPhysicSystem(uint flags);
void quitPhysicSystem();
errorCode getPhysicHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);


// audio
bool initAudioSystem(uint flags);
void quitAudioSystem();
errorCode getAudioHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);


} // end namespace Engine

#endif // BACKEND_HPP
