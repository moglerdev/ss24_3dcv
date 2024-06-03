//
//  A very simple class for rudimentary scene management
//
//  (c) Georg Umlauf, 2021+2022
//
#include "QtConvenience.h"

#include "Scene.h"
#include "PointCloud.h"
#include "Cube.h"
#include "StereoVision.h"

#define CLOUD_MODE 1

//
// iterates all objects under its control and has them drawn by the renderer
//
void Scene::draw(const RenderCamera &renderer, const QColor &color) const
{
    for (auto obj : *this)
        if (obj)
        {
            switch (obj->getType())
            {

            case SceneObjectType::ST_AXES:
                obj->draw(renderer, COLOR_AXES, 2.0f);
                break;
            case SceneObjectType::ST_POINT_CLOUD:
                obj->draw(renderer, COLOR_POINT_CLOUD, 3.0f); // last argument unused
                break;
#if CLOUD_MODE == 0
            case SceneObjectType::ST_PLANE:
                obj->draw(renderer, COLOR_PLANE, 0.3f);
                break;
            case SceneObjectType::ST_CUBE:
            case SceneObjectType::ST_HEXAHEDRON:
                obj->draw(renderer, color, 2.0f);
                break;
            case SceneObjectType::ST_PERSPECTIVECAMERA:
            {
                // TODO: Assignement 1, Part 3
                // This is the place to invoke the perspective camera's projection method and draw the projected objects.
                PerspectiveCamera *camera = dynamic_cast<PerspectiveCamera *>(obj);
                camera->draw(renderer, COLOR_CAMERA, 3.0f);
                for (auto obj2 : *this)
                {
                    camera->renderSceneObject(renderer, obj2);
                }
                break;
            }
            case SceneObjectType::ST_STEREO_VISION:
            {
                // TODO: Assignement 2, Part 1 - 3
                // Part 1: This is the place to invoke the stereo camera's projection method and draw the projected objects.
                // Part 2: This is the place to invoke the stereo camera's reconstruction method.
                // Part 3: This is the place to invoke the stereo camera's reconstruction method using misaligned stereo cameras.
                StereoVision *vision = dynamic_cast<StereoVision *>(obj);
                for (auto obj2 : *this)
                {
                    vision->renderSceneObject(renderer, obj2);
                }
                break;
            }
#endif
            default:
                break;
            }
        }
}
