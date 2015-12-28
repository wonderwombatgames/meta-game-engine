/**
  * helper functions to abstrct the backend
  *
  */

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

namespace Engine
{
template< class Impl >
class Texture
{
public:
  static Texture * LoadTexture(const char * filepath, const char * atlas = nullptr);

protected:
  Impl _data;
};

} // end namespace Engine

#endif // TEXTURE_HPP
