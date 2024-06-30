//
// Widget für Interaktion und Kontrolle
//
// (c) Nico Brügel, 2021
// (c) Georg Umlauf, 2021+2022+2024
//
#pragma once

#include <QOpenGLWidget>

#include "Renderer.h" // containes declaration of Renderer
#include "Scene.h"    // containes declaration of Scene Manager
#include "PerspectiveCamera.h"

#define CLOUD_MODE 1
#define MODE 2

class GLWidget : public QOpenGLWidget
{
    Q_OBJECT
private:
    // scene and scene control
    int pointSize;
    Scene scene;
    PerspectiveCamera *camera;
    PerspectiveCamera *stereo;
    float delta;
    float phi;
    float psi;
    void initAffineMaps();
    void initPCA();

public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget() Q_DECL_OVERRIDE;

public slots:
    // button + slider controls
    void openFileDialog();         // opens and loads a PLY file to a point cloud
    void radioButtonClicked();     // handles radio buttons
    void checkBoxClicked();        // handle check boxes
    void spinBoxValueChanged(int); // handles spin  boxes changes
    void setPointSize(int);
    void setPerspectiveCameraX(double);
    void setPerspectiveCameraY(double);
    void setPerspectiveCameraZ(double);
    void setPerspectiveCameraPsi(double);
    void setPerspectiveCameraPhi(double);
    void setPerspectiveCameraTheta(double);

protected:
    // painting the canvas
    void paintGL() Q_DECL_OVERRIDE;
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

protected:
    // mouse + wheel + key navigation
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;    // handles key-press   events
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;  // handles key-release events
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;     // handles wheel       events
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE; // handles mouse-move  events

private slots:
    // handle changes of the renderer
    void onRendererChanged();

private:
    // interaction control
    bool X_Pressed = false;
    bool Y_Pressed = false;
    QPoint prevMousePosition;

    // rendering control
    RenderCamera *renderer = nullptr;
};
