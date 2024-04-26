#include <QMatrix4x4>

QMatrix4x4 R_x(const float rad)
{
  auto cos = std::cos(rad);
  auto sin = std::sqrt(1 - cos * cos);
  return QMatrix4x4(1, 0, 0, 0,
                    0, cos, -sin, 0,
                    0, sin, cos, 0,
                    0, 0, 0, 1);
}

QMatrix4x4 R_y(const float rad)
{
  auto cos = std::cos(rad);
  auto sin = std::sqrt(1 - cos * cos);
  return QMatrix4x4(cos, 0, sin, 0,
                    0, 1, 0, 0,
                    -sin, 0, cos, 0,
                    0, 0, 0, 1);
}

QMatrix4x4 R_z(const float rad)
{
  auto cos = std::cos(rad);
  auto sin = std::sqrt(1 - cos * cos);
  return QMatrix4x4(cos, -sin, 0, 0,
                    sin, cos, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
}
QMatrix4x4 rotate(const QVector3D &vec)
{
  // 𝑅𝑧 𝜃 𝑅𝑦(𝜙)𝑅𝑧 𝜓 𝑅𝑦(−𝜙)𝑅𝑧(−𝜃)

  return R_z(-vec.z()) + R_y(vec.y()) + R_z(vec.z());
}
