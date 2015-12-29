/**
  * helper functions to abstrct the backend
  *
  */

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <memory>

namespace Engine
{
using namespace std;

class Impl;

class Texture
{
public:
  static Texture * LoadTexture(const char * filepath, const char * atlas = nullptr);

protected:
  unique_ptr< Impl > _data;
};

} // end namespace Engine

#endif // TEXTURE_HPP
