/**
  * helper functions to abstrct the backend
  *
  */

#ifndef BACKEND_HPP
#define BACKEND_HPP

#include "basic_types.hpp"


namespace Engine
{

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
// bool initGraphicSystem(Flags flags = 0);
// void quitGraphicSystem();
ErrorCode getGraphicHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);

// inputs functions
// bool initInputSystem(Flags flags = 0);
// void quitInputSystem();
ErrorCode getInputHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);

// events functions
// bool initEventSystem(Flags flags = 0);
// void quitEventSystem();
ErrorCode getEventHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);

// physics functions
// bool initPhysicSystem(Flags flags = 0);
// void quitPhysicSystem();
ErrorCode getPhysicHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);

// audio
bool initAudioSystem(Flags flags = 0);
void quitAudioSystem();
ErrorCode getAudioHandler(IHandler * handler = nullptr, const IConfig * data = nullptr);


} // end namespace Engine

#endif // BACKEND_HPP
