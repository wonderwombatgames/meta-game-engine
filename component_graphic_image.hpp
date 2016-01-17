/**
  * helper functions to abstrct the backend
  *
  */

#ifndef COMPONENT_GRAPHIC_IMAGE_HPP
#define COMPONENT_GRAPHIC_IMAGE_HPP

#include "utils_colour.hpp"
#include "component_graphic.hpp"

namespace W2E
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
    Image(const String & filepath);
    // TODO:
    // Image(const ImageAtlas & atlas, vector<AtlasKey> keys);
    // Image(const NetworkResource & netRes);
    virtual ~Image();

    bool isLoaded();
    ErrorCode loadFromFile(const String & filepath);
    // TODO:
    // ErrorCode loadFromAtlas(const ImageAtlas & atlas);
    // ErrorCode loadFromNet(const NetworkResource & netRes);

    virtual void paint(
        const GraphicPod & component,
        const Vector3 & offset = { .x = 0.0f, .y = 0.0f, .z = 0.0f}) override;
    // virtual bool setParameter(
    //     const char * paramName,
    //     const float & paramValue = 0.0f) override;

  protected:
    using _HANDLER = T;
    UniquePtr< _HANDLER > _data;

    // size - between 0.0 - 1.0 (in relation to viewport size)
    Dimension2 _textureSize;

    void computeClipRects(const GraphicPod & component, BoxBoundXYWH & src, BoxBoundXYWH & dst, Vector2 & center);
  };

} // end namespace Component

} // end namespace W2E

#endif // COMPONENT_GRAPHIC_IMAGE_HPP
