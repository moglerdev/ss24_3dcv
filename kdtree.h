#ifndef KDTREE_H
#define KDTREE_H

#include "kdnode.h"
#include "QColor"

class KdTree
{

public:
    KdTree(QVector<QVector4D> *data);
    KdTree();
    ~KdTree();

    KdNode *root;
    QVector<QVector3D> X;
    QVector<QVector3D> Y;
    QVector<QVector3D> Z;

    QVector<std::pair<std::pair<QVector3D, QVector3D>, QColor>> lines;
    QVector<std::pair<QVector3D, QColor>> points;

    void construct(int level);
    void construct(int l, int r, KdNode *k, int d, int level);
    void partition(QVector<QVector3D> list, QVector3D median, int l, int r, int m, int d);

    struct sort_after_x
    {
        bool operator()(QVector3D point1, QVector3D point2)
        {
            return (point1.x() < point2.x());
        }
    };

    struct sort_after_y
    {
        bool operator()(QVector3D point1, QVector3D point2)
        {
            return (point1.y() < point2.y());
        }
    };

    struct sort_after_z
    {
        bool operator()(QVector3D point1, QVector3D point2)
        {
            return (point1.z() < point2.z());
        }
    };
};

#endif // KDTREE_H
