#ifndef OCTTREE_H
#define OCTTREE_H

#include <QVector>

#include "octnode.h"

class OctTree
{
public:
    OctTree(QVector<QVector4D> *data);
    OctTree();
    ~OctTree();

    OctNode* root;
    std::vector<QVector3D> X;
    std::vector<QVector3D> Y;
    std::vector<QVector3D> Z;
    std::vector<QVector3D> test;

    void generate_tree();
    void construct_tree(std::vector<QVector3D> list_of_points, QVector3D corner_1,QVector3D corner_2, OctNode* &k);
    void construct();
    std::vector<OctNode> get_all_level_nodes(int level);
    void level_traversal(std::vector<OctNode> *list, OctNode *node, int level);

    struct sort_after_x
    {
        inline bool operator() (const QVector3D point1, const QVector3D point2)
        {
            return (point1.x() < point2.x());
        }
    };

    struct sort_after_y
    {
        inline bool operator() (const QVector3D point1, const QVector3D point2)
        {
            return (point1.y() < point2.y());
        }
    };

    struct sort_after_z
    {
        inline bool operator() (const QVector3D point1, const QVector3D point2)
        {
            return (point1.z() < point2.z());
        }
    };
};


#endif // OCTTREE_H
