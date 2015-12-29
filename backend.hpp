/**
  * helper functions to abstrct the backend
  *
  */

#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <cstdint>

namespace Engine
{

typedef uint32_t Flags;
typedef uint8_t ErrorCode;

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
bool initGraphicSystem(Flags flags);
void quitGraphicSystem();
ErrorCode getGraphicHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);

// inputs functions
bool initInputSystem(Flags flags);
void quitInputSystem();
ErrorCode getInputHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);

// events functions
bool initEventSystem(Flags flags);
void quitEventSystem();
ErrorCode getEventHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);

// physics functions
bool initPhysicSystem(Flags flags);
void quitPhysicSystem();
ErrorCode getPhysicHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);

// audio
bool initAudioSystem(Flags flags);
void quitAudioSystem();
ErrorCode getAudioHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);


} // end namespace Engine

#endif // BACKEND_HPP
