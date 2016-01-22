/**
  * base entities data component
  */

#ifndef GRAPHIC_COMPONENT_HPP
#define GRAPHIC_COMPONENT_HPP

#include "utils_colour.hpp"
#include "component_transform.hpp"

namespace W2E
{
using namespace Utils;

namespace Component
{
// forward declaration
class GraphicInterface;

struct GraphicPod
{

  // defines the anchor within the boudaries
  // values between 0.0 - 1.0 (in relation to entity size | UV)
  Vector3 anchor = CENTER;

  // colour parameters
  Colour colourTint = Colours::WHITE;
  ColourComp alphaMode = 1.0f;
  eBlendMode blendingMode = BLENDMODEALPHA_;

  // whether to show the entity or not
  bool isVisible = true;

  // reference to transform data component
  TransformPod* transformData = nullptr;

  // graphic element data pointer
  GraphicInterface* resource = nullptr;
};

class GraphicInterface
{
public:
  GraphicInterface() {}
  virtual ~GraphicInterface() {}
  virtual void paint(const GraphicPod& component, const TransformPod& transformData) = 0;
};

template < typename T >
class Image : public GraphicInterface
{
public:
  Image(Image& other) = delete;
  Image();
  Image(const String& filepath);
  // TODO:
  // Image(const ImageAtlas & atlas, vector<AtlasKey> keys);
  // Image(const NetworkResource & netRes);
  virtual ~Image();

  bool isLoaded();
  ErrorCode loadFromFile(const String& filepath);
  // TODO:
  // ErrorCode loadFromAtlas(const ImageAtlas & atlas);
  // ErrorCode loadFromNet(const NetworkResource & netRes);

  virtual void paint(const GraphicPod& component, const TransformPod& transformData) override;
  // virtual bool setParameter(
  //     const char * paramName,
  //     const float & paramValue = 0.0f) override;

protected:
  using HANDLER_ = T;
  UniquePtr< HANDLER_ > data_;

  // size - between 0.0 - 1.0 (in relation to viewport size)
  Dimension2 textureSize_;
};

} // namespace Component

} // end namespace W2E

#endif // GRAPHIC_COMPONENT_HPP
