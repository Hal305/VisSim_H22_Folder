#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWindow>
#include <QOpenGLFunctions_4_1_Core>
#include <QTimer>
#include <QElapsedTimer>
#include <vector>
#include "input.h"
#include "quadtree.h"


class QOpenGLContext;
class Vertex;
class VisualObject;
class Camera;
class Input;
class Shader;
class MainWindow;
class Logger;
class Light;
class XYZ;
class TriangleSurface;
class OctaBall;
class RollingBall;

/// This inherits from QWindow to get access to the Qt functionality and
// OpenGL surface.
// We also inherit from QOpenGLFunctions, to get access to the OpenGL functions
// This is the same as using "glad" and "glw" from general OpenGL tutorials
class RenderWindow : public QWindow, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow);
    ~RenderWindow() override;

    QOpenGLContext *context() { return mContext; }

    void exposeEvent(QExposeEvent *) override;  //gets called when app is shown and resized

    void setupShader(int index);

    bool bWireFrame {false};

private slots:
    void render();          //the actual render - function

private:
    std::vector<VisualObject*> mObjects;                    //Standard container
    std::unordered_map<std::string, VisualObject*> mMap;    //Hash container
    gsml::QuadTree<std::string, VisualObject*> mQuadTree;   //Quadtree

    VisualObject* surface {nullptr};
    VisualObject* ball {nullptr};
    Input mInput;
    Camera* mCamera {nullptr};
    float aspectratio = 1.f;
    bool bCameraLock {false};

    void init();            //initialize things we need before rendering

    QOpenGLContext *mContext{nullptr};  //Our OpenGL context
    bool mInitialized{false};

    std::vector<Shader*> mShaders;    //holds pointer the GLSL shader program
    static const int uniforms = 2;
    std::vector<GLint> mMMatrixUniform;          //OpenGL reference to the Uniform in the shader program
    std::vector<GLint> mVMatrixUniform;
    std::vector<GLint> mPMatrixUniform;
    GLint mTextureUniform{-1};

    Light* mLight {nullptr};
    //other light shader variables
    GLint mLightColorUniform{-1};
    GLint mObjectColorUniform{-1};
    GLint mAmbientLightStrengthUniform{-1};
    GLint mLightPositionUniform{-1};
    GLint mCameraPositionUniform{-1};
    GLint mSpecularStrengthUniform{-1};
    GLint mSpecularExponentUniform{-1};
    GLint mLightPowerUniform{-1};
    GLint mTextureUniform2{-1};

    GLuint mVAO;                        //OpenGL reference to our VAO
    GLuint mVBO;                        //OpenGL reference to our VBO

    QMatrix4x4 *mMMatrix{nullptr};          //The matrix with the transform for the object we draw

    QTimer *mRenderTimer{nullptr};           //timer that drives the gameloop
    QElapsedTimer mTimeStart;               //time variable that reads the calculated FPS

    MainWindow *mMainWindow{nullptr};        //points back to MainWindow to be able to put info in StatusBar

    class QOpenGLDebugLogger *mOpenGLDebugLogger{nullptr};  //helper class to get some clean debug info from OpenGL
    class Logger *mLogger{nullptr};         //logger - Output Log in the application

    ///Helper function that uses QOpenGLDebugLogger or plain glGetError()
    void checkForGLerrors();

    void calculateFramerate();          //as name says

    ///Starts QOpenGLDebugLogger if possible
    void startOpenGLDebugger();

protected:
    //The QWindow that we inherit from have these functions to capture
    // - mouse and keyboard.
    void movePlayer();
    // Uncomment to use (you also have to make the definitions of
    // these functions in the cpp-file to use them of course!)
    //
    //    void mousePressEvent(QMouseEvent *event) override{}
    //    void mouseMoveEvent(QMouseEvent *event) override{}
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    //    void wheelEvent(QWheelEvent *event) override{}
};

#endif // RENDERWINDOW_H
