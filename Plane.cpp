//
// (c) Georg Umlauf, 2021
//
#include "Plane.h"

#include "GLConvenience.h"
#include "QtConvenience.h"

Plane::Plane(const QVector4D &_origin, const QVector4D &_normal) : origin(_origin), normal(_normal)
{
    type = SceneObjectType::ST_PLANE;
    normal.normalize();
}

void Plane::affineMap(const QMatrix4x4 &M)
{
    origin = M * origin;
    normal = M * normal;
    normal.normalize();
}

Plane &Plane::operator=(const Plane &p)
{
    if (this != &p)
    {
        origin = p.origin;
        normal = p.normal;
    }
    return *this;
}

std::array<QVector3D, 4> Plane::getVertices() const
{
    QVector3D n(normal);
    QVector3D o(origin);
    QVector3D y = QVector3D::crossProduct(e1, n);
    if (y.length() < 0.001f)
        y = QVector3D::crossProduct(e2, n);
    QVector3D x = QVector3D::crossProduct(-y, n);
    x.normalize();
    y.normalize();

    std::array<QVector3D, 4> vertices;
    vertices[0] = o + 1.0f * x + 1.0f * y;
    vertices[1] = o - 1.0f * x + 1.0f * y;
    vertices[2] = o - 1.0f * x - 1.0f * y;
    vertices[3] = o + 1.0f * x - 1.0f * y;

    return vertices;
}

void Plane::draw(const RenderCamera &renderer, const QColor &color, float transparency) const
{
    std::array<QVector3D, 4> vertices = getVertices();
    renderer.renderPlane(vertices[0], vertices[1], vertices[2], vertices[3], color, 0.05f);
    renderer.renderLine(origin, origin + normal, color, 3.0f);
}
