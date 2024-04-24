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
    Plane imagePlane;

public:
    PerspectiveCamera(
        const QMatrix4x4 &_pose,
        const QVector4D &_imagePrincipalPoint,
        const Plane &_imagePlane) : pose(_pose),
                                    imagePrincipalPoint(_imagePrincipalPoint),
                                    imagePlane(_imagePlane)
    {
        imagePlane.affineMap(_pose);
        type = SceneObjectType::ST_PERSPECTIVECAMERA;
    }

    virtual ~PerspectiveCamera() override
    {
    }

    void renderPoint(const RenderCamera &renderer, const QVector4D &worldPoint)
    {
        renderer.renderLine(pose.column(3), worldPoint, COLOR_CAMERA);
        renderer.renderPoint(worldPoint, COLOR_AXES, 20.0f);
        // calculate the relative position of the point in the camera coordinate system
        auto relativePoint = pose.inverted() * worldPoint;

        // calculate eucliden distance from imagePrincipalPoint to camera
        auto dis = pose.inverted() * imagePrincipalPoint;
        // invert it because we use it for the front
        auto f = -dis.z();

        // calculate perspective projection
        auto planePoint = (-f/relativePoint.z()) * QVector2D(relativePoint.x(), relativePoint.y());

        // calculate real world point cordinates
        auto point = pose * QVector4D(planePoint.x(), planePoint.y(), dis.z(), 1);

        renderer.renderPoint(point, COLOR_PLANE, 20.0f);
    }

    //void renderLine(const RenderCamera &renderer, const QVector4D &a, const QVector4D &b) {
    //    renderer.renderPoint(worldPoint, COLOR_AXES, 20.0f);
    //
    //}

    // apply affine map to the corners of the hexahedron
    virtual void affineMap(const QMatrix4x4 &matrix) override
    {

    }

    // draws the wireframe of the hexahedron
    virtual void draw(const RenderCamera &renderer,
                      const QColor &color = COLOR_SCENE,
                      float lineWidth = 3.0f) const override;
};

#endif // PERSPECTIVECAMERA_H
