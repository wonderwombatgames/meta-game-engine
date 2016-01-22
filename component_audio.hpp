/**
  * structs containing component data
  *
  */

#ifndef AUDIO_COMPONENT_HPP
#define AUDIO_COMPONENT_HPP

#include "utils_types.hpp"

namespace W2E
{
using namespace Utils;

namespace Component
{

// forward declaration
class MusicData;
class SoundFxData;

struct Audio
{
  MusicData* music;
  float musicVolume; // values between 0.0 - 1.0
  eAudioState musicState;
  SoundFxData* soundFx;
  float soundFxVolume; // values between 0.0 - 1.0
  eAudioState soundFxState;
};

} // namespace Component

} // end namespace W2E

#endif // AUDIO_COMPONENT_HPP
