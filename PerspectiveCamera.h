#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include <math.h>

#include "SceneObject.h"
#include "Plane.h"

static QMatrix4x4 R_z(const QVector4D &vec, const float x, const float y)
{
    QMatrix4x4 res;
    float d = sqrt(x * x + y * y);

    res(0, 0) = x;
    res(1, 0) = -y;
    res(0, 1) = y;
    res(1, 1) = x;
    res(2, 2) = d;
    res(3, 3) = d;

    return (1 / d) * res;
}

static QMatrix4x4 R_y(const QVector4D &vec, const float x, const float z)
{
    QMatrix4x4 res;
    float d = sqrt(x * x + z * z);

    res(0, 0) = z;
    res(0, 2) = -d;
    res(2, 2) = z;
    res(2, 0) = d;
}

static QMatrix4x4 RotateMatrix(const QVector4D &vec, const float theta, const float phi, const float psi)
{
    float x = vec.x();
    float y = vec.y();
    float z = vec.z();

    // Step 1

    QMatrix4x4 R_z_theta;
    float d_xy = sqrt(x * x + y * y);

    R_z_theta(0, 0) = x;
    R_z_theta(0, 1) = -y;
    R_z_theta(1, 0) = y;
    R_z_theta(1, 1) = x;
    R_z_theta(2, 2) = d_xy;
    R_z_theta(3, 3) = d_xy;

    R_z_theta = R_z_theta * 1 / d_xy;

    // Step 2

    QMatrix4x4 R_y_phi;
    R_y_phi.setToIdentity();
    float d_xz = sqrt(x * x + z * z);
    R_y_phi(0, 0) = z;
    R_y_phi(0, 2) = -d_xz;
    R_y_phi(2, 0) = d_xz;
    R_y_phi(2, 2) = z;

    // Step 3

    QMatrix4x4 R_z_psi;
    R_z_psi.setToIdentity();
    float cos_psi = cos(-psi);
    float sin_psi = sqrt(1 - cos_psi * cos_psi);
    R_z_psi(0, 0) = cos_psi;
    R_z_psi(0, 1) = -sin_psi;
    R_z_psi(1, 0) = sin_psi;
    R_z_psi(1, 1) = cos_psi;

    // Step 4

    QMatrix4x4 R_y_phi;
}

class PerspectiveCamera : public SceneObject
{
protected:
    QMatrix4x4 pose;
    QVector4D imagePrincipalPoint;
    Plane plane;

public:
    PerspectiveCamera(
        const QMatrix4x4 &_pose,
        const QVector4D &_imagePrincipalPoint,
        const Plane &_plane) : pose(_pose),
                               imagePrincipalPoint(_imagePrincipalPoint),
                               plane(_plane)
    {
    }

    virtual ~PerspectiveCamera() override
    {
    }

    virtual QMatrix4x4 translateVector(const QVector4D &vec)
    {
        return QMatrix4x4(); // TODO:
    }

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
