#ifndef STEREOVISIONWITHERROR_H
#define STEREOVISIONWITHERROR_H

#include "StereoVision.h"
#include "PerspectiveCamera.h"
#include <cmath>
#include <QVector4D>
#include <QMatrix4x4>
#include <stdexcept>
#include <QDebug>

class StereoVisionWithError : public StereoVision
{
private:
    double error_px = 0;
    double error_x = 0;
    double error_y = 0;

public:
    StereoVisionWithError(PerspectiveCamera* _camera, PerspectiveCamera* _stereo, double _error_px = 0, double _error_x = 0, double _error_y = 0)
        : StereoVision(_camera, _stereo), error_px(_error_px), error_x(_error_x), error_y(_error_y)  {
    }

    QVector4D calculateStereoPoint(const QVector4D &camLocal, const QVector4D &steLocal) override {
        auto f = this->camera->getF().z();
        auto camPose = this->camera->getPose();
        auto stePose = this->stereo->getPose();

        auto b = std::abs(stePose.column(3).x() - camPose.column(3).x());

        auto parallax = camLocal.x() - steLocal.x();
        auto z = f * (b / parallax);
        auto y = z * (steLocal.y() / f);
        auto x = z * (steLocal.x() / f);

        auto m_b = z / f;
        auto m_v = b / z;
        auto m_b_div_m_v = m_b/m_v;
        auto sigma_z = m_b_div_m_v * error_px;

        auto sy_1 = (camLocal.y()/f)*m_b_div_m_v*error_px;
        auto sy_2 = m_b * error_y;
        auto sigma_y = std::sqrt(sy_1 * sy_1 + sy_2 * sy_2);
        auto sx_1 = (camLocal.x()/f)*m_b_div_m_v*error_px;
        auto sx_2 = m_b * error_y;
        auto sigma_x = std::sqrt(sx_1 * sx_1 + sx_2 * sx_2);

        return stePose.inverted() * QVector4D(x + sigma_x, y + sigma_y, z + sigma_z, 1);
    }
};

#endif // STEREOVISIONWITHERROR_H
