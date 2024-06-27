#ifndef OCTNODE_H
#define OCTNODE_H

#include "QVector3D""

class OctNode
{
public:
    OctNode();
    ~OctNode();

    QVector3D point;
    QVector3D topLeftFront;
    QVector3D bottomRightBack;
    int level;
    std::vector<OctNode*> children;
};

#endif // OCTNODE_H
