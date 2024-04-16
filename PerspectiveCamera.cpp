#include "PerspectiveCamera.h"

void PerspectiveCamera::draw(
    const RenderCamera &renderer,
    const QColor &color,
    float transparency) const
{
  // draw the camera origin

  renderer.renderPoint(this->pose.column(3), color, 20.0f);
  renderer.renderPoint(this->imagePrincipalPoint, color, 20.0f);
  // renderer.renderPoint(this->origin, color, 20.0f);
}
