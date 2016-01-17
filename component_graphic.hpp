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
    Vector3 anchor;// = { .x = 0.5f, .y = 0.5f, .zorder = 0.0f};

    // colour parameters
    Colour colourTint;
    ColourComp alphaMode = 1.0f;
    BlendingMode blendingMode = 0;

    // whether to show the entity or not
    bool isVisible;// = true;

    // reference to transform data component
    TransformPod * transformData = nullptr;

    // graphic element data pointer
    GraphicInterface * resource = nullptr;

    // GraphicPod()
    //     :transformData(nullptr)
    //     ,anchor{0.5f, 0.5f, 0.0f}
    //     ,reource(nullptr)
    //     ,colourTint()
    //     ,alphaMode(1.0f)
    //     ,blendingMode(0)
    //     ,isVisible(true)
    // {}
  };


  class GraphicInterface
  {
  public:
    GraphicInterface(){}
    virtual ~GraphicInterface(){}
    virtual void
      paint(const GraphicPod & component, const Vector3 & offset = {{ 0.0f, 0.0f, 0.0f }} ) = 0;
    // virtual bool setParameter(const char * paramName, const float & paramValue = 0.0f) = 0;
  };

} // namespace Component

} // end namespace W2E

#endif // GRAPHIC_COMPONENT_HPP
