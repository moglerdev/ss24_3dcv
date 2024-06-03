#include "kdtree.h"

KdTree::KdTree(QVector<QVector4D> *data) : lines(*new QVector<std::pair<std::pair<QVector3D, QVector3D>, QColor>>), points(*new QVector<std::pair<QVector3D, QColor>>)
{

    for (auto point : *data)
    {
        X.push_back(point.toVector3D());
        Y.push_back(point.toVector3D());
        Z.push_back(point.toVector3D());
    }

    std::sort(X.begin(), X.end(), sort_after_x());
    std::sort(Y.begin(), Y.end(), sort_after_y());
    std::sort(Z.begin(), Z.end(), sort_after_z());

    root = NULL;
}

KdTree::KdTree() : lines(*new QVector<std::pair<std::pair<QVector3D, QVector3D>, QColor>>),
                   points(*new QVector<std::pair<QVector3D, QColor>>), root(NULL)
{
}

KdTree::~KdTree() {}

void KdTree::construct(int level)
{
    construct(0, X.size() - 1, root, 0, level);
}

void KdTree::construct(int l, int r, KdNode *k, int d, int level)
{
    if (level <= 0)
    {
        return;
    }

    k = new KdNode();

    if (l <= r)
    {
        int m = ceil((l + r) / 2);
        if (d == 0)
        { // vertical
            k->point = Y[m];

            points.push_back(std::make_pair(QVector3D(Y[m].x(), Y[m].y(), Y[m].z()), QColor(255, 0, 0)));

            partition(X, Y[m], l, r, m, 1); // y-split
        }
        else if (d == 1)
        { // horizontal
            k->point = X[m];

            points.push_back(std::make_pair(QVector3D(X[m].x(), X[m].y(), X[m].z()), QColor(0, 255, 0)));

            partition(Z, X[m], l, r, m, 0); // x-split
        }
        else
        { // depth
            k->point = Z[m];

            points.push_back(std::make_pair(QVector3D(Z[m].x(), Z[m].y(), Z[m].z()), QColor(0, 0, 255)));

            partition(Y, Z[m], l, r, m, 2); // z-split
        }

        d = (d + 1) % 3;

        construct(l, m - 1, k->left, d, level - 1);
        construct(m + 1, r, k->right, d, level - 1);
    }
}

void KdTree::partition(QVector<QVector3D> list, QVector3D median, int l, int r, int m, int d)
{
    QVector<QVector3D> tmp1, tmp2;
    for (int i = l; i <= r; i++)
    {
        if (d == 0)
        { // x-split
            if (list[i].x() < median.x())
                tmp1.push_back(list[i]);
            if (list[i].x() > median.x())
                tmp2.push_back(list[i]);
        }
        else if (d == 1)
        { // y-split
            if (list[i].y() < median.y())
                tmp1.push_back(list[i]);
            if (list[i].y() > median.y())
                tmp2.push_back(list[i]);
        }
        else if (d == 2)
        { // z-split
            if (list[i].z() < median.z())
                tmp1.push_back(list[i]);
            if (list[i].z() > median.z())
                tmp2.push_back(list[i]);
        }
    }
    for (int i = 0; i < tmp1.size(); i++)
        list[l + i] = tmp1[i];
    for (int i = 0; i < tmp2.size(); i++)
        list[m + i + 1] = tmp2[i];
}
