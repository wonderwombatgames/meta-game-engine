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
  eBlendMode blendingMode = BLENDMODE_ALPHA;

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

} // namespace Component

} // end namespace W2E

#endif // GRAPHIC_COMPONENT_HPP
