#include "PerspectiveCamera.h"

void PerspectiveCamera::draw(
    const RenderCamera &renderer,
    const QColor &color,
    float transparency) const
{
  // draw the camera origin

  renderer.renderPoint(this->pose.column(3), color, 20.0f);
  renderer.renderPoint(this->imagePrincipalPoint, color, 20.0f);

  // auto cornerTopLeft = this->imagePlane.getCornerTopLeft();
  // renderer.renderLine(this->pose.column(3), cornerTopLeft, color, 3.0f);
  // renderer.renderPoint(this->origin, color, 20.0f);
}
