#include "camera.h"

Camera::Camera()
{

}

void Camera::init()
{
    initializeOpenGLFunctions();
    mPMatrix.setToIdentity();
    mVMatrix.setToIdentity();
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
    return mPosition;
}

void Camera::translate(float dx, float dy, float dz)
{
    mVMatrix.translate(dx, dy, dz);
}


// Ole's class
//Camera::Camera()
//{
//    mViewMatrix.setToIdentity();
//    mProjectionMatrix.setToIdentity();

//    mYawMatrix.setToIdentity();
//    mPitchMatrix.setToIdentity();
//}

//void Camera::pitch(float degrees)
//{
//    //  rotate around mRight
//    mPitch -= degrees;
//    updateForwardVector();
//}

//void Camera::yaw(float degrees)
//{
//    // rotate around mUp
//    mYaw -= degrees;
//    updateForwardVector();
//}

//void Camera::updateRightVector()
//{
//    mRight = QVector3D::crossProduct(mForward, mUp);
//    mRight.normalize();
////    qDebug() << "Right " << mRight;
//}

//void Camera::updateForwardVector()
//{
//    mRight = QVector3D(1.f, 0.f, 0.f);
//    mRight.rotateY(mYaw);
//    mRight.normalize();
//    mUp = QVector3D(0.f, 1.f, 0.f);
//    mUp.rotateX(mPitch);
//    mUp.rotateY(mYaw);
//    mUp.normalize();
//    mForward = QVector3D::crossProduct(mUp, mRight);

//    updateRightVector();
//}

//void Camera::update()
//{
//    mYawMatrix.setToIdentity();
//    mPitchMatrix.setToIdentity();

//    mPitchMatrix.rotate(mPitch, mUp);
//    mYawMatrix.rotate(mYaw, mRight);

//    mPosition -= mForward * mSpeed;

//    mViewMatrix = mPitchMatrix* mYawMatrix;
//    mViewMatrix.translate(-mPosition);
//}

//void Camera::setPosition(const QVector3D &position)
//{
//    mPosition = position;
//    update();
//}

//void Camera::setSpeed(float speed)
//{
//    mSpeed = speed;
//}

//void Camera::updateHeigth(float deltaHeigth)
//{
//    mPosition.setY(mPosition.y() + deltaHeigth);
//}

//void Camera::moveRight(float delta)
//{
//    //This fixes a bug in the up and right calculations
//    //so camera always holds its height when straifing
//    //should be fixed thru correct right calculations!
//    QVector3D right = mRight;
//    right.setY(0.f);
//    mPosition += right * delta;
//}

//QVector3D Camera::position() const
//{
//    return mPosition;
//}

//QVector3D Camera::up() const
//{
//    return mUp;
//}

//void Vector3D::rotateX(GLfloat angle)
//    {
//        Vector3D dir;
//        angle = gsl::deg2rad(angle);

//        dir.setX(x);
//        dir.setY((y * std::cos(angle)) + (z * (-std::sin(angle))));
//        dir.setZ((y * std::sin(angle)) + (z * std::cos(angle)));

//        x = dir.getX();
//        y = dir.getY();
//        z = dir.getZ();
//    }

//    void Vector3D::rotateY(GLfloat angle)
//    {
//        Vector3D dir;
//        angle = gsl::deg2rad(angle);

//        dir.setX((x * std::cos(angle)) + (z * std::sin(angle)));
//        dir.setY(y);
//        dir.setZ((x * (-std::sin(angle))) + (z * std::cos(angle)));

//        x = dir.getX();
//        y = dir.getY();
//        z = dir.getZ();
//    }

//    void Vector3D::rotateZ(GLfloat angle)
//    {
//        Vector3D dir;
//        angle = gsl::deg2rad(angle);

//        dir.setX((x * std::cos(angle)) + (y * (-std::sin(angle))));
//        dir.setY((x * std::sin(angle)) + (y * std::cos(angle)));
//        dir.setZ(z);

//        x = dir.getX();
//        y = dir.getY();
//        z = dir.getZ();
//    }
