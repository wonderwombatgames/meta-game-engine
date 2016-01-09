/**
  * helper functions to abstrct the backend
  *
  */

#ifndef COMPONENT_GRAPHIC_IMAGE_HPP
#define COMPONENT_GRAPHIC_IMAGE_HPP

#include <memory>
#include <string>
#include <vector>

#include "component_graphic.hpp"

namespace Engine
{
using namespace std;
using namespace Utils;

// TODO:
// forward declaration
// class ImageAtlas;
// class NetworkResource;

namespace Component
{

  template <typename T >
  class Image : public GraphicInterface
  {
  public:
    Image(Image & other) = delete;
    Image();
    Image(const string & filepath);
    // TODO:
    // Image(const ImageAtlas & atlas, vector<AtlasKey> keys);
    // Image(const NetworkResource & netRes);
    virtual ~Image();

    bool isLoaded();
    bool loadFromFile(const string & filepath);
    // TODO:
    // bool loadFromAtlas(const ImageAtlas & atlas);
    // bool loadFromNet(const NetworkResource & netRes);

    virtual void paint(
        const GraphicPod & component,
        const Vector3 & offset = {0.0f, 0.0f, 0.0f}) override;
    virtual bool setParameter(
        const char * paramName,
        const float & paramValue = 0.0f) override;

  protected:
    typedef T _HANDLER;
    unique_ptr< _HANDLER > _data;

    // size - between 0.0 - 1.0 (in relation to viewport size)
    Dimension3 _textureSize;

    void computeClipRects(const GraphicPod & component, BoxBoundXYWH & src, BoxBoundXYWH & dst, Vector3 & center);
  };

} // end namespace Component

} // end namespace Engine

#endif // COMPONENT_GRAPHIC_IMAGE_HPP
