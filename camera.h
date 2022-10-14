#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <QVector3D>

class Camera : public QOpenGLFunctions_4_1_Core
{
public:
    Camera();
    ~Camera(){};
    void init();
    void perspective(int degrees, double aspect, double nearplane, double farplane);
    void frustum();
    void lookAt(const QVector3D& eye, const QVector3D& at, const QVector3D& up);
    void update();
    void translate(float dx, float dy, float dz);
    QMatrix4x4 mVMatrix{};
    QMatrix4x4 mPMatrix{};
    QVector3D position();

private:
    QVector3D mPosition{0.f, 0.f, 3.f};

    QVector3D mForward{0.f, 0.f, -1.f};
    QVector3D mRight{1.f, 0.f, 0.f};
    QVector3D mUp{0.f, 1.f, 0.f};

    float mPitch{0.f};
    float mYaw{0.f};

    QMatrix4x4 mYawMatrix;
    QMatrix4x4 mPitchMatrix;
    float mSpeed{0.f};
};

// Ole's class
//class Camera
//{
//public:
//    Camera();

//    void pitch(float degrees);
//    void yaw(float degrees);
//    void updateRightVector();
//    void updateForwardVector();
//    void update();

//    QMatrix4x4 mViewMatrix;
//    QMatrix4x4 mProjectionMatrix;

//    void setPosition(const QVector3D &position);

//    void setSpeed(float speed);
//    void updateHeigth(float deltaHeigth);
//    void moveRight(float delta);

//    QVector3D position() const;
//    QVector3D up() const;

//private:
//    QVector3D mForward{0.f, 0.f, -1.f};
//    QVector3D mRight{1.f, 0.f, 0.f};
//    QVector3D mUp{0.f, 1.f, 0.f};

//    QVector3D mPosition{0.f, 0.f, 0.f};
//    float mPitch{0.f};
//    float mYaw{0.f};

//    QMatrix4x4 mYawMatrix;
//    QMatrix4x4 mPitchMatrix;

//    float mSpeed{0.f}; //camera will move by this speed along the mForward vector
//};

#endif // CAMERA_H
