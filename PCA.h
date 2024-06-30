#ifndef PCA_H
#define PCA_H

#include <Axes.h>
#include <PointCloud.h>

class PCA : public SceneObject
{
public:
  PCA(PointCloud *sourcePointcloud, PointCloud *targetPointcloud);
  ~PCA();
  void align();
  virtual void affineMap(const QMatrix4x4 &matrix) override;
  virtual void draw(const RenderCamera &renderer, const QColor &color, float lineWidth) const override;

private:
  QMatrix4x4 createTranslationMatrix(float x, float y, float z) const;
  QMatrix4x4 createRotationMatrix(float angleX, float angleY, float angleZ) const;
  float multiplyRows(std::vector<float> row1, std::vector<float> row2) const;
  float angleFromMatrix(QMatrix4x4 matrix) const;
  std::pair<QVector4D, QMatrix4x4> calculateEigen(PointCloud *pc) const;
  std::vector<float> eigvals(QMatrix4x4 matrix) const;
  std::vector<float> solve_cubic_equation(float a, float b, float c, float d) const;
  std::vector<QVector3D> eigvecs(QMatrix4x4 matrix, std::vector<float> vals) const;

  PointCloud *source;
  PointCloud *target;
};

#endif // PCA_H
