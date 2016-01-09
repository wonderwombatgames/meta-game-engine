/**
  * base entities data component
  */

#ifndef GRAPHIC_COMPONENT_HPP
#define GRAPHIC_COMPONENT_HPP

#include "component_transform.hpp"

namespace Engine
{
using namespace Utils;

namespace Component
{

// forward declaration
  class GraphicInterface;

  struct GraphicPod
  {
    // reference to transform data component
    TransformPod * transformData;

    // defines the anchor within the boudaries
    // values between 0.0 - 1.0 (in relation to entity size | UV)
    Vector3 anchor;

    // graphic element data pointer
    GraphicInterface * element;

    // colour parameters
    Colour colourTint;
    ColourComponent alphaMode;
    BlendingMode blendingMode;

    // whether to show the entity or not
    bool isVisible;

  };

  // Colour c;
  // c.kind = RGBA;
  // c.rgba = {0.0f, 0.0f, 0.0f, 1.0f};
  // Component::GraphicPod pod{
  //         // reference to entity transform component
  //         transform,
  //         // defines the anchor within the boudaries
  //         // values between 0.0 - 1.0 (in relation to entity size | UV)
  //         {0.5f, 0.5f, 0.0f},
  //         // graphic element data pointer
  //         nullptr,
  //         // colour parameters
  //         c,    // Colour colourTint
  //         1.0f, // ColourComponent alphaMode;
  //         0,    // BlendingMode blendingMode;
  //         // whether to show the entity or not
  //         true                // isVisible

  class GraphicInterface
  {
  public:
    GraphicInterface(){}
    virtual ~GraphicInterface(){}
    virtual void
      paint(const GraphicPod & component, const Vector3 & offset = {0.0f, 0.0f, 0.0f}) = 0;
    virtual bool setParameter(const char * paramName, const float & paramValue = 0.0f) = 0;
  };

} // namespace Component

} // end namespace Engine

#endif // GRAPHIC_COMPONENT_HPP
