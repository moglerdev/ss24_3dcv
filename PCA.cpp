#include <iostream>
#include "external/eigen-3.3.9/Eigen/Eigen"
#include "PCA.h"

PCA::PCA() noexcept {}

QVector4D PCA::calculate_center(PointCloud *pts)
{
    std::vector<QVector4D> points_vector;
    for (auto p : *pts)
    {
        points_vector.push_back(p);
    }
    QVector4D center(0, 0, 0, 0);
    for (QVector4D point : points_vector)
    {
        center += point;
    }
    center /= points_vector.size();
    return center;
}

QMatrix4x4 PCA::calculate_axis_matrix(PointCloud *pts, const QVector4D &center)
{
    std::vector<QVector3D> points_vector;
    for (auto p : *pts)
    {
        points_vector.push_back(p.toVector3D());
    }
    // Matrix A aufbauen. A=((p1-c) (p2-c) (p3-c) (p4-c)...(pn-c))
    size_t n = points_vector.size(); // Anzahl der Punkte
    std::vector<std::vector<float>> A(3, std::vector<float>(n));
    for (size_t j = 0; j < n; ++j)
    {
        for (int i = 0; i < 3; ++i)
        {
            A[i][j] = points_vector[j][i] - center[i];
        }
    }

    // Matrix A transponieren
    std::vector<std::vector<float>> A_transposed(n, std::vector<float>(3));
    for (size_t i = 0; i < 3; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            A_transposed[j][i] = A[i][j];
        }
    }

    // Matrix M aufbauen M = A * A^T
    std::vector<std::vector<float>> M(4, std::vector<float>(3, 0.0f));
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (size_t k = 0; k < n; ++k)
            {
                M[i][j] += A[i][k] * A_transposed[k][j];
            }
        }
    }

    // Matrix M ausgeben
    std::cout << "Matrix M = A * A^T:" << std::endl;
    for (const auto &row : M)
    {
        for (float val : row)
        {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // M in Eigen Matrix umwandeln
    Eigen::Matrix3f eigen_matrix;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            eigen_matrix(i, j) = M[i][j];
        }
    }

    // Eigenvektoren berechnen
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> solver(eigen_matrix);
    Eigen::Matrix3f eigenvectors = solver.eigenvectors();
    Eigen::Vector3f eigenvalues = solver.eigenvalues();

    // make pairs of eigenvalues and eigenvectors
    std::vector<std::pair<float, Eigen::Vector3f>> eigen_pairs;
    for (int i = 0; i < 3; ++i)
    {
        eigen_pairs.push_back(std::make_pair(eigenvalues(i), eigenvectors.col(i)));
    }

    // sort the eigen pairs according to the eigenvalues in ascending order
    std::sort(eigen_pairs.begin(), eigen_pairs.end(), [](const std::pair<float, Eigen::Vector3f> &p1, const std::pair<float, Eigen::Vector3f> &p2)
              { return p1.first < p2.first; });

    // print pairs
    for (const auto &pair : eigen_pairs)
    {
        std::cout << "Eigenvalue: " << pair.first << std::endl;
        std::cout << "Eigenvector:\n"
                  << pair.second << std::endl;
    }

    return QMatrix4x4(eigen_pairs[0].second(0), eigen_pairs[1].second(0), eigen_pairs[2].second(0), 0,
                      eigen_pairs[0].second(1), eigen_pairs[1].second(1), eigen_pairs[2].second(1), 0,
                      eigen_pairs[0].second(2), eigen_pairs[1].second(2), eigen_pairs[2].second(2), 0,
                      0, 0, 0, 1);
}

int PCA::determinant(QMatrix4x4 matrix)
{
    return matrix(0, 0) * (matrix(1, 1) * matrix(2, 2) - matrix(1, 2) * matrix(2, 1)) -
           matrix(0, 1) * (matrix(1, 0) * matrix(2, 2) - matrix(1, 2) * matrix(2, 0)) +
           matrix(0, 2) * (matrix(1, 0) * matrix(2, 1) - matrix(1, 1) * matrix(2, 0));
}
