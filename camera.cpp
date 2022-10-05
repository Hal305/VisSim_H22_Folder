#include "camera.h"

Camera::Camera()
{

}

void Camera::init()
{
    initializeOpenGLFunctions();
    mPMatrix.setToIdentity();
    mVMatrix.setToIdentity();
    mYawMatrix.setToIdentity();
    mPitchMatrix.setToIdentity();
}

void Camera::perspective(int degrees, double aspect, double nearplane, double farplane)
{
    mPMatrix.perspective(degrees, aspect, nearplane, farplane);
}

void Camera::lookAt(const QVector3D &eye, const QVector3D &at, const QVector3D &up)
{
    mVMatrix.lookAt(eye, at, up);
}

void Camera::update()
{

}

QVector3D Camera::position()
{
    QVector3D pos = {mPosition.column(3).x() ,mPosition.column(3).y(), mPosition.column(3).z()};
    return pos;
}

void Camera::translate(float dx, float dy, float dz)
{
    //Change yaw
//    mYawMatrix.rotate(dx*-20, 0,0,-1);//Angle and z-axis rotation from x-axis input
    //Direction calculations, let's the camera move in direction it's facing
//    dx = dy*-(mYawMatrix.column(0).x(), mYawMatrix.column(1).x()); //cos angle - sin angle
//    dy *= (mYawMatrix.column(0).y(), mYawMatrix.column(1).y());   //sin angle + cos angle
//    mPosition.setColumn(3, {mx += dx, my += dy, mz+=dz, 1.0});
//    mVMatrix = mPosition * mYawMatrix;
    mVMatrix.translate(dx, dy, dz);
}
