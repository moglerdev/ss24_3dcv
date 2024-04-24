#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include <math.h>

#include "SceneObject.h"
#include "Plane.h"

class PerspectiveCamera : public SceneObject
{
protected:
    QMatrix4x4 pose;
    QVector4D imagePrincipalPoint;
    Plane *imagePlane;

public:
    PerspectiveCamera(
        const QMatrix4x4 &_pose,
        const QVector4D &_imagePrincipalPoint,
        Plane *_imagePlane) : pose(_pose),
                              imagePlane(_imagePlane)
    {
        imagePlane->affineMap(_pose);
        this->imagePrincipalPoint = pose * _imagePrincipalPoint;
        type = SceneObjectType::ST_PERSPECTIVECAMERA;
    }

    virtual ~PerspectiveCamera() override
    {
    }

    QVector4D calculateProjectedPoint(const QVector4D &worldPoint)
    {
        // calculate the relative position of the point in the camera coordinate system
        auto relativePoint = pose.inverted() * worldPoint;

        // calculate eucliden distance from imagePrincipalPoint to camera
        auto dis = pose.inverted() * imagePrincipalPoint;
        // invert it because we use it for the front
        auto f = -dis.z();

        // calculate perspective projection
        auto planePoint = (-f / relativePoint.z()) * QVector2D(relativePoint.x(), relativePoint.y());

        // calculate real world point cordinates
        return pose * QVector4D(planePoint.x(), planePoint.y(), dis.z(), 1);

        // renderer.renderPoint(point, COLOR_CAMERA, 20.0f);
    }

    void renderLine(const RenderCamera &renderer, const QVector4D &a, const QVector4D &b)
    {
        // TODO: keine Ahnung ob das so richtig ist, aber es geht xD

        renderer.renderLine(a, b, COLOR_CAMERA, 3.0f);

        auto t = this->calculateProjectedPoint(a);
        auto v = this->calculateProjectedPoint(b);

        renderer.renderLine(pose.column(3), a, COLOR_POINT_CLOUD, 1.0f);
        renderer.renderLine(pose.column(3), b, COLOR_POINT_CLOUD, 1.0f);

        renderer.renderLine(t, v, COLOR_CAMERA, 3.0f);
    }

    // apply affine map to the corners of the hexahedron
    virtual void affineMap(const QMatrix4x4 &matrix) override
    {
        pose = matrix * pose;
        imagePrincipalPoint = pose * imagePrincipalPoint;
        imagePlane->affineMap(pose);
    }

    // draws the wireframe of the hexahedron
    virtual void draw(const RenderCamera &renderer,
                      const QColor &color = COLOR_SCENE,
                      float lineWidth = 3.0f) const override;
};

#endif // PERSPECTIVECAMERA_H
