#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include <math.h>

#include "SceneObject.h"
#include "Plane.h"
#include "Hexahedron.h"
#include "PointCloud.h"

class PerspectiveCamera : public SceneObject
{
protected:
    QMatrix4x4 pose;
    QVector4D imagePrincipalPoint;
    Plane *imagePlane;

public:
    PerspectiveCamera(
        const QMatrix4x4& _pose,
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

    QMatrix4x4 getPose() const
    {
        return pose;
    }

    void setPose(const QMatrix4x4& newPose)
    {
        auto affineMap = pose.inverted() * newPose;
        pose = newPose;
        // IMPORTANT IS THAT THE AFFINE MAP IS BEFORE THE VECTOR !!!!!
        imagePrincipalPoint = affineMap * imagePrincipalPoint;
        this->imagePlane->affineMap(affineMap);
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
    }

    void renderLine(const RenderCamera &renderer, const QVector4D &a, const QVector4D &b)
    {
        // TODO: keine Ahnung ob das so richtig ist, aber es geht xD

        auto t = this->calculateProjectedPoint(a);
        auto v = this->calculateProjectedPoint(b);

        renderer.renderLine(t, v, COLOR_CAMERA, 3.0f);
    }

    void renderHexahedron(const RenderCamera &renderer, const Hexahedron *hexahedron)
    {
        bool first = true;
        QVector4D lastPoint;
        for (auto p : *hexahedron)
        {
            auto t = this->calculateProjectedPoint(QVector4D(p, 1.0f));
            if (!first)
            {
                renderer.renderLine(lastPoint, t, COLOR_CAMERA, 1.0f);
            }
            lastPoint = t;
            first = false;
            renderer.renderPoint(t, COLOR_CAMERA, 10.0f);
        }
    }

    void renderPointCloud(const RenderCamera &renderer, const PointCloud &pointCloud)
    {
        for (auto p : pointCloud)
        {
            auto t = this->calculateProjectedPoint(p);
            renderer.renderPoint(t, COLOR_CAMERA, 5.0f);
        }
    }

    void renderSceneObject(const RenderCamera &renderer, const SceneObject *sceneObject)
    {
        switch (sceneObject->getType())
        {
        case SceneObjectType::ST_CUBE:
        case SceneObjectType::ST_HEXAHEDRON:
            this->renderHexahedron(renderer, dynamic_cast<const Hexahedron *>(sceneObject));
            break;
        case SceneObjectType::ST_POINT_CLOUD:
            this->renderPointCloud(renderer, dynamic_cast<const PointCloud &>(*sceneObject));
            break;
        }
    }

    // apply affine map to the corners of the hexahedron
    virtual void affineMap(const QMatrix4x4 &matrix) override
    {
        pose = matrix * pose;
        imagePrincipalPoint = pose * imagePrincipalPoint;
        imagePlane->affineMap(matrix);
    }

    // draws the wireframe of the hexahedron
    virtual void draw(const RenderCamera &renderer,
                      const QColor &color = COLOR_SCENE,
                      float lineWidth = 3.0f) const override;
};

#endif // PERSPECTIVECAMERA_H
