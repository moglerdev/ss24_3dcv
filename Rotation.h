#include <QMatrix4x4>

QMatrix4x4 R_z(float deg)
{
    auto rad = deg * M_PI / 180;
    auto cos_rad = std::cos(rad);
    auto sin_rad = std::sqrt(1 - cos_rad * cos_rad);

    return QMatrix4x4(cos_rad, -sin_rad, 0, 0,
                      sin_rad, cos_rad, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1);
}

QMatrix4x4 R_y(float deg)
{
    auto rad = deg * M_PI / 180;
    auto cos_rad = std::cos(rad);
    auto sin_rad = std::sqrt(1 - cos_rad * cos_rad);

    return QMatrix4x4(cos_rad, 0, sin_rad, 0,
                      0, 1, 0, 0,
                      -sin_rad, 0, cos_rad, 0,
                      0, 0, 0, 1);
}

QMatrix4x4 R_x(float deg)
{
    auto rad = deg * M_PI / 180;
    auto cos_rad = std::cos(rad);
    auto sin_rad = std::sqrt(1 - cos_rad * cos_rad);

    return QMatrix4x4(1, 0, 0, 0,
                      0, cos_rad, -sin_rad, 0,
                      0, sin_rad, cos_rad, 0,
                      0, 0, 0, 1);
}

QMatrix4x4 getRotationMatrix(QVector3D rotation)
{
    return R_z(rotation.x()) * R_y(rotation.y()) * R_z(rotation.z());
}
