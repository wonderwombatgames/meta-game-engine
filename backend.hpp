/**
  * helper functions to abstrct the backend
  *
  */

#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <cstdint>

namespace Engine
{

typedef uint8_t errorCode;

class IHandler
{
public:
  virtual ~IHandler(){}
};

class IConfig
{
public:
  virtual ~IConfig(){}
};


// graphics functions
bool initGraphicSystem(uint16_t flags);
void quitGraphicSystem();
errorCode getGraphicHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);

// inputs functions
bool initInputSystem(uint16_t flags);
void quitInputSystem();
errorCode getInputHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);

// events functions
bool initEventSystem(uint16_t flags);
void quitEventSystem();
errorCode getEventHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);

// physics functions
bool initPhysicSystem(uint16_t flags);
void quitPhysicSystem();
errorCode getPhysicHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);


// audio
bool initAudioSystem(uint16_t flags);
void quitAudioSystem();
errorCode getAudioHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);


} // end namespace Engine

#endif // BACKEND_HPP
