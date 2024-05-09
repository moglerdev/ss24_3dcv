#ifndef STEREOVISION_H
#define STEREOVISION_H

#include "SceneObject.h"
#include "PerspectiveCamera.h"

class StereoVision : public SceneObject
{
private:
    PerspectiveCamera* camera;
    PerspectiveCamera* stereo;

public:
    StereoVision(PerspectiveCamera* _camera, PerspectiveCamera* _stereo) : camera(_camera), stereo(_stereo) {
        type = SceneObjectType::ST_STEREO_VISION;
    }


    // apply affine map to the corners of the hexahedron
    virtual void affineMap(const QMatrix4x4&) override
    {
        // DO NOTHING, BECAUSE CALCULATE ONLY STEREO VISION
    }

    QVector4D calculateStereoPoint(const QVector4D &cameraPointPos, const QVector4D &stereoPointPos) {
        auto f = this->camera->getF().z();
        if (f != this->stereo->getF().z()) {
            throw "camera and stereo have wrong f";
        }

        auto camPose = this->camera->getPose();
        auto stePose = this->stereo->getPose();

        auto z = -f * camPose.column(3).z();
        // TODO: FERTIG HIER !!! Folie §3 / 22; 3dv.03.xopp
    }

    void renderHexahedron(const RenderCamera &renderer, const Hexahedron *hexahedron)
    {
        const std::vector<QVector3D> &corners = std::vector(*hexahedron);
        for (unsigned i = 0; i < 2 * hexahedron->getEdgeCount(); i += 2) {
            auto org1 = corners[hexahedron->getEdgeList()[i]];
            auto org2 = corners[hexahedron->getEdgeList()[i + 1]];

            auto cam1 = this->camera->calculateProjectedPoint(QVector4D(org1));
            auto cam2 = this->camera->calculateProjectedPoint(QVector4D(org2));

            auto ste1 = this->stereo->calculateProjectedPoint(QVector4D(org1));
            auto ste2 = this->stereo->calculateProjectedPoint(QVector4D(org2));

            // TODO: Calculate Stereo Vision Points
        }
    }

    void renderPointCloud(const RenderCamera &renderer, const PointCloud &pointCloud)
    {
        for (auto p : pointCloud)
        {
            auto t = this->calculateProjectedPoint(p);
            renderer.renderPoint(t, COLOR_CAMERA, 5.0f);
        }
    }

    void renderSceneObject(const RenderCamera &renderer, const SceneObject *sceneObject)
    {
        switch (sceneObject->getType())
        {
        case SceneObjectType::ST_CUBE:
        case SceneObjectType::ST_HEXAHEDRON:
            this->renderHexahedron(renderer, dynamic_cast<const Hexahedron *>(sceneObject));
            break;
        case SceneObjectType::ST_POINT_CLOUD:
            this->renderPointCloud(renderer, dynamic_cast<const PointCloud &>(*sceneObject));
            break;
        }
    }

    // draws the wireframe of the hexahedron
    virtual void draw(const RenderCamera &renderer,
                      const QColor &color = COLOR_SCENE,
                      float lineWidth = 3.0f) const override {
        // GET
    }
};

#endif // STEREOVISION_H
