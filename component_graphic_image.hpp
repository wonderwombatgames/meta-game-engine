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
//class ImageAtlas;
//class AtlasKey;

namespace Component
{

  template <typename T >
  class Image : public IGraphic
  {
  public:
    Image(Graphic & component);
    Image(Graphic & component, const string & filepath);
    //Image(const ImageAtlas & atlas, vector<AtlasKey> keys);
    Image() = delete;
    Image(Image & other) = delete;
    virtual ~Image();

    bool loadFromFile(const string & filepath);
    //bool loadFromAtlas(const ImageAtlas & atlas, vector<AtlasKey> keys)
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
    typedef T _HANDLER;

    Graphic * _component;
    unique_ptr< _HANDLER > _data;

    void init();
    void getWindowSize(int & w, int & h);
    void getImageSize(int & w, int & h);
    void computeClipRects(BoxBoundXYWH & src, BoxBoundXYWH & dst, Vector3 & center);
  };

} // end namespace Component

} // end namespace Engine

#endif // TEXTURE_HPP
