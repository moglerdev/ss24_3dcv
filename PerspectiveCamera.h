#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "SceneObject.h"
#include "Plane.h"

class PerspectiveCamera : public SceneObject
{
protected:
    QVector4D origin;
    float dx, dy, dz;
    Plane *plane;
    std::vector<SceneObject *> objects;

public:
    PerspectiveCamera(
        const QVector4D &_origin,
        Plane *_plane,
        std::vector<SceneObject *> _objects);

    void addSceneObject(SceneObject *object) { objects.push_back(object); }

    virtual ~PerspectiveCamera() override {}

    // apply affine map to the corners of the hexahedron
    virtual void affineMap(const QMatrix4x4 &matrix) override;
    // draws the wireframe of the hexahedron
    virtual void draw(const RenderCamera &renderer,
                      const QColor &color = COLOR_SCENE,
                      float lineWidth = 3.0f) const override;
};

#endif // PERSPECTIVECAMERA_H
