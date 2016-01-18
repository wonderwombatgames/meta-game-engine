/**
  * transform data component
  */

#ifndef TRANSFORM_COMPONENT_HPP
#define TRANSFORM_COMPONENT_HPP

#include "utils_types.hpp"

namespace W2E
{
using namespace Utils;

namespace Component
{

  struct TransformPod
  {

    // position
    // absolute in pixels (float values)
    // can contain z-order for 2d spaces
    Vector3 position;

    // rotation
    // values between 0.0 - 1.0  (= 0 - 360)
    Rotation3 rotation;

    // scales
    // <1.0 = smaller | > 1.0 = larger
    // <0.0 = mirror
    // this attribure is shared by physics and
    // graphics, and might have application
    // to the entity behaviour as well
    Vector3 scale;

    // kind of space 2D/3D
    eSpace kind;

    // TransformPod()
    // : position{{ 0.0f, 0.0f, 0.0f }}
    // , rotation{{ 0.0f, 0.0f, 0.0f }}
    // , scale{{ 1.0f, 1.0f, 1.0f }}
    // , kind(SPACE_2D) {}
    //
    // TransformPod(TransformPod & other) = default;
    // TransformPod(TransformPod && other) = default;
    // TransformPod & operator=(TransformPod & other) = default;
  };


} // namespace Component

} // end namespace W2E

#endif // TRANSFORM_COMPONENT_HPP
