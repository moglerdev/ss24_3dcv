#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(
    const QVector4D &_origin,
    Plane *_plane,
    std::vector<SceneObject *> _objects) : origin(_origin),
                                           plane(_plane),
                                           objects(_objects)
{
}

void PerspectiveCamera::affineMap(
    const QMatrix4x4 &matrix)
{
  origin = matrix * origin;
  plane->affineMap(matrix);
  for (unsigned i = 0; i < objects.size(); i++)
    objects[i]->affineMap(matrix);
}

void PerspectiveCamera::draw(
    const RenderCamera &renderer,
    const QColor &color,
    float transparency) const
{
  // draw the camera origin
  renderer.renderPoint(E0, COLOR_CAMERA);
}
