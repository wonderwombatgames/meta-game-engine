/**
  * helper functions to abstrct the backend
  *
  */

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <memory>
#include <string>
#include "component_graphic.hpp"

namespace Engine
{
using namespace std;
using namespace Utils;

// forward declaration
//class TextureAtlas;
//class AtlasKey;

namespace Component
{

  template <typename T >
  class Texture : public IGraphic
  {
  public:
    Texture(Graphic & component);
    Texture(Graphic & component, const string & filepath);
    //Texture(const TextureAtlas & atlas, vector<AtlasKey> keys);
    Texture() = delete;
    Texture(Texture & other) = delete;
    virtual ~Texture();

    bool loadFromFile(const string & filepath);
    //bool loadFromAtlas(const TextureAtlas & atlas, vector<AtlasKey> keys)
    bool isLoaded();

    virtual void paint(const Vector3 & offset = {0.0f, 0.0f, 0.0f}) override;

  #if 0
   void setPosition(const Vector3 & p);
   const Vector3 & getPosition();
    void setScale(const Vector3 & s);
    const Vector3 & getScale();
    void setAnchor(const Vector3 & a);
    const Vector3 & getAnchor();
    void setRotation(const Rotation3 & r);
    const Rotation3 & getRotation();
    void setTinting();
    const Colour & getTinting();
    void setAlpha(const ColourComponent & a);
    const ColourComponent & getAlpha();
    void setBlendMode(const BlendingMode & b);
    const BlendingMode & getBlendMode();
    void setAtlasFrame(const unsigned short & frame);
    const unsigned short & getAtlasFrame();
  #endif

  protected:
    typedef T Context;

    Graphic * _component;
    unique_ptr< Context > _data;

    void init();
    void getWindowSize(int & w, int & h);
    void getTextureSize(int & w, int & h);
    void computeClipRects(BoxBoundXYWH & src, BoxBoundXYWH & dst, Vector3 & center);
  };

} // end namespace Component

} // end namespace Engine

#endif // TEXTURE_HPP
