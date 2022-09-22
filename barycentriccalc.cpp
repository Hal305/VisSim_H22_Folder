#include "barycentriccalc.h"

BarycentricCalc::BarycentricCalc(QVector2D point)
{
    mPoint = point;
}

QVector3D BarycentricCalc::calculate(const QVector2D &p1, const QVector2D &p2, const QVector2D &p3)
{
    //Eksempel uke 10.1: p1 = (0, 1), p2 = (1.5, 0), p3 = (2.5, 1), pX = (0.5, 0.5)
    QVector2D p12 = p2-p1; //x1 = (1.5, -1)
    QVector2D p13 = p3-p1; //x2 = (2.5, 0)

    //-1 * 0 - (0 * 0), 0 * 2.5 - (1.5 * 0), 1.5 * 0 - (-1 * 2.5)
    QVector3D n = QVector3D::crossProduct(p12.toVector3D(),p13.toVector3D());//= (0, 0, 2.5), or 2.5
    float area123 = n.z(); // double area, 2.5
    QVector3D baryc; //return value
    QVector2D P = p1 - mPoint; //= (-0.5, 0.5)
    QVector2D Q = p2 - mPoint; //= (1, -0.5)
    QVector2D R = p3 - mPoint; //= (2, 0.5)
    //Cross products
    // u: Q x R = 1 * 0.5 - (-0.5 * 2)
    n = QVector3D::crossProduct(Q.toVector3D(), R.toVector3D()); //= 1.5
    baryc.setX(n.z()/area123); //u = 1.5/2.5 = 0.6
    // v: R x P = 2 * 0.5 -(0.5 * -0.5)
    n = QVector3D::crossProduct(R.toVector3D(), P.toVector3D()); //= 1.25
    baryc.setY(n.z()/area123); // v = 1.25/2.5 = 0.5
    // w: P x Q = -0.5 * -0.5 -(0.5 * 1)
    n = QVector3D::crossProduct(P.toVector3D(), Q.toVector3D()); //= -0.25
    baryc.setZ(n.z()/area123); //w = -0.1
    //u + v + w = 0.6 + 0.5 - 0.1 = 1
    return baryc;
}
