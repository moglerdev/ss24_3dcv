#include "PCA.h"
#include <Eigen/Dense>
#include "math.h"
#include "glwidget.h"

PCA::PCA(PointCloud *sourcePoibtcloud, PointCloud *targetPointcloud) : source(sourcePoibtcloud), target(targetPointcloud)
{
    target->affineMap(createTranslationMatrix(0.5, 0.5, 0.5));
    target->affineMap(createRotationMatrix(30.0, 0.0, 0.0));
    type = SceneObjectType::ST_PCA;
}

PCA::~PCA()
{
}

void PCA::align()
{
    // First: Centroid, Second: Rotation-Matrix
    auto source_ret = calculateEigen(source);
    auto target_ret = calculateEigen(target);
    auto rotation = source_ret.second * target_ret.second.transposed();
    target->affineMap(rotation);

    source_ret = calculateEigen(source);
    target_ret = calculateEigen(target);
    auto translation = (source_ret.first.toVector3D() - target_ret.first.toVector3D()).toVector4D();
    target->affineMap(createTranslationMatrix(translation.x(), translation.y(), translation.z()));
}

void PCA::draw(const RenderCamera &renderer, const QColor &color, float lineWidth) const
{
    source->draw(renderer, QColor(255, 255, 255), 3.0);
    auto f = calculateEigen(source);
    (new Axes(f.first, f.second))->draw(renderer);
    target->draw(renderer, QColor(255, 0, 0), 3.0);

    auto s = calculateEigen(target);
    (new Axes(s.first, s.second))->draw(renderer);
}

void PCA::affineMap(const QMatrix4x4 &M) {}

QMatrix4x4 PCA::createTranslationMatrix(float x, float y, float z) const
{
    return QMatrix4x4(
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1);
}

QMatrix4x4 PCA::createRotationMatrix(float angleX, float angleY, float angleZ) const
{
    QMatrix4x4 rotationMatrix;

    rotationMatrix.rotate(angleX, QVector3D(1.0f, 0.0f, 0.0f));
    rotationMatrix.rotate(angleY, QVector3D(0.0f, 1.0f, 0.0f));
    rotationMatrix.rotate(angleZ, QVector3D(0.0f, 0.0f, 1.0f));

    return rotationMatrix;
}

std::pair<QVector4D, QMatrix4x4> PCA::calculateEigen(PointCloud *pc) const
{
    float cx = 0, cy = 0, cz = 0;
    for (auto pnt = pc->begin(); pnt != pc->end(); ++pnt)
    {
        auto point = *pnt;
        cx += point.x();
        cy += point.y();
        cz += point.z();
    }
    int m = pc->size();
    QVector3D centroid = QVector3D(cx / m, cy / m, cz / m);

    std::vector<float> px, py, pz;
    for (auto pnt = pc->begin(); pnt != pc->end(); ++pnt)
    {
        auto point = *pnt;
        px.push_back(point.x() - centroid.x());
        py.push_back(point.y() - centroid.y());
        pz.push_back(point.z() - centroid.z());
    }

    std::vector<float> vals;
    std::vector<QVector3D> vecs;

    QMatrix4x4 matrix = QMatrix4x4();
    QMatrix4x4 mat;

    mat = QMatrix4x4(multiplyRows(px, px), multiplyRows(px, py), multiplyRows(px, pz), 0,
                     multiplyRows(py, px), multiplyRows(py, py), multiplyRows(py, pz), 0,
                     multiplyRows(pz, px), multiplyRows(pz, py), multiplyRows(pz, pz), 0,
                     0, 0, 0, 1);

    vals = eigvals(mat);
    vecs = eigvecs(mat, vals);

    matrix.setColumn(0, vecs[0].toVector4D());
    matrix.setColumn(1, vecs[1].toVector4D());
    matrix.setColumn(2, vecs[2].toVector4D());
    matrix.setColumn(3, QVector4D(0, 0, 0, 1));

    return std::make_pair(centroid.toVector4D(), matrix);
}

float PCA::multiplyRows(std::vector<float> row1, std::vector<float> row2) const
{
    float sum = 0;
    for (int i = 0; i < row1.size(); i++)
    {
        sum += row1[i] * row2[i];
    }
    return sum;
}

std::vector<float> PCA::eigvals(QMatrix4x4 matrix) const
{
    std::vector<float> vals;
    float a = matrix.row(0).x();
    float b = matrix.row(0).y();
    float c = matrix.row(0).z();
    float d = matrix.row(1).x();
    float e = matrix.row(1).y();
    float f = matrix.row(1).z();
    float g = matrix.row(2).x();
    float h = matrix.row(2).y();
    float i = matrix.row(2).z();

    float trace = a + e + i;
    float determinant = (a * e * i - a * f * h) + (b * f * g - b * d * i) + (c * d * h - c * e * g);

    std::vector<float> px = {matrix.row(0).x(), matrix.row(0).y(), matrix.row(0).z()};
    std::vector<float> py = {matrix.row(1).x(), matrix.row(1).y(), matrix.row(1).z()};
    std::vector<float> pz = {matrix.row(2).x(), matrix.row(2).y(), matrix.row(2).z()};

    QMatrix4x4 second_matrix = QMatrix4x4(multiplyRows(px, px), multiplyRows(px, py), multiplyRows(px, pz), 0,
                                          multiplyRows(py, px), multiplyRows(py, py), multiplyRows(py, pz), 0,
                                          multiplyRows(pz, px), multiplyRows(pz, py), multiplyRows(pz, pz), 0,
                                          0, 0, 0, 1);
    float second_trace = second_matrix.row(0).x() + second_matrix.row(1).y() + second_matrix.row(2).z();
    std::vector<float> test = solve_cubic_equation(-1, trace, -0.5 * (trace * trace - second_trace), determinant);
    return solve_cubic_equation(-1, trace, -0.5 * (trace * trace - second_trace), determinant);
}

std::vector<float> PCA::solve_cubic_equation(float a, float b, float c, float d) const
{
    std::vector<float> roots;

    float p = (b * b - 3 * a * c);
    float q = (9. * a * b * c - 2 * (b * b * b) - 27 * (a * a) * d);
    float theta = acos(q / ((2 * p) * sqrt(p)));
    float root1 = (-b + 2 * cos(theta / 3) * sqrt(p)) / (3 * a);
    float root2 = (-b + 2 * cos((theta / 3) + (120. * (M_PI / 180))) * sqrt(p)) / (3 * a);
    float root3 = (-b + 2 * cos((theta / 3) + (240. * (M_PI / 180))) * sqrt(p)) / (3 * a);

    roots.push_back(root1);
    roots.push_back(root2);
    roots.push_back(root3);
    std::sort(roots.begin(), roots.end(), std::greater{});
    return roots;
}

std::vector<QVector3D> PCA::eigvecs(QMatrix4x4 matrix, std::vector<float> vals) const
{
    std::vector<QVector3D> vecs;

    float a = matrix.row(0).x();
    float b = matrix.row(0).y();
    float c = matrix.row(0).z();
    float d = matrix.row(1).x();
    float e = matrix.row(1).y();
    float f = matrix.row(1).z();
    float g = matrix.row(2).x();
    float h = matrix.row(2).y();
    float i = matrix.row(2).z();

    for (auto val : vals)
    {
        std::vector<float> row0 = {a - val, b, c};
        std::vector<float> row1 = {d, e - val, f};
        std::vector<float> row2 = {g, h, i - val};

        QVector3D r0xr1 = QVector3D(row0[1] * row1[2] - row0[2] * row1[1],
                                    row0[2] * row1[0] - row0[0] * row1[2],
                                    row0[0] * row1[1] - row0[1] * row1[0]);
        QVector3D r0xr2 = QVector3D(row0[1] * row2[2] - row0[2] * row2[1],
                                    row0[2] * row2[0] - row0[0] * row2[2],
                                    row0[0] * row2[1] - row0[1] * row2[0]);
        QVector3D r1xr2 = QVector3D(row1[1] * row2[2] - row1[2] * row2[1],
                                    row1[2] * row2[0] - row1[0] * row2[2],
                                    row1[0] * row2[1] - row1[1] * row2[0]);
        float d0 = r0xr1[0] * r0xr1[0] + r0xr1[1] * r0xr1[1] + r0xr1[2] * r0xr1[2];
        float d1 = r0xr2[0] * r0xr2[0] + r0xr2[1] * r0xr2[1] + r0xr2[2] * r0xr2[2];
        float d2 = r1xr2[0] * r1xr2[0] + r1xr2[1] * r1xr2[1] + r1xr2[2] * r1xr2[2];

        float index_max = 0;
        float d_max = d0;

        if (d1 > d_max)
        {
            d_max = d1;
            index_max = 1;
        }
        if (d2 > d_max)
        {
            d_max = d2;
            index_max = 2;
        }

        if (index_max == 0)
        {
            vecs.push_back(r0xr1 / sqrt(d0));
        }
        if (index_max == 1)
        {
            vecs.push_back(r0xr2 / sqrt(d1));
        }
        if (index_max == 2)
        {
            vecs.push_back(r1xr2 / sqrt(d2));
        }
    }
    return vecs;
}
