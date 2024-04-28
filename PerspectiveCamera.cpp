#include "PerspectiveCamera.h"

void PerspectiveCamera::draw(
    const RenderCamera &renderer,
    const QColor &color,
    float) const
{
  // draw the camera origin

  renderer.renderPoint(this->pose.column(3), color, 20.0f);

  // draw the camera axes
  auto planeVertices = this->imagePlane->getVertices();
  auto pos = pose.column(3);
  renderer.renderLine(pos, QVector4D(planeVertices[0]), COLOR_POINT_CLOUD, 3.0f);
  renderer.renderLine(pos, QVector4D(planeVertices[1]), COLOR_POINT_CLOUD, 3.0f);
  renderer.renderLine(pos, QVector4D(planeVertices[2]), COLOR_POINT_CLOUD, 3.0f);
  renderer.renderLine(pos, QVector4D(planeVertices[3]), COLOR_POINT_CLOUD, 3.0f);
}
