#ifndef PCA_H
#define PCA_H

#include "PointCloud.h"
#include <QVector4D>
#include <QMatrix4x4>

class PCA
{

  PCA() noexcept;
  
  QVector4D calculate_center(PointCloud *pts);

  QMatrix4x4 calculate_axis_matrix(PointCloud *pts, const QVector4D &center);

  int determinant(QMatrix4x4 matrix);
};

#endif // PCA_H
