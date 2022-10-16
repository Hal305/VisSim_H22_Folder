#include "tpsurface.h"

TPSurface::TPSurface()
{

}

//std::pair<int, int> TPSurface::findMy(float tu, float tv)
//{

//}

std::pair<QVector3D,QVector3D> TPSurface::B2(float tu, float tv, int my_u, int my_v)
{
// Lagrer B-spline koeffisientene i en 3d vektor her (d=2)
// Bv.x = B_{my_v-2}, Bv.y = B_{my_v-1}, Bv.z = B_{my_v}
// Bu.x = B_{my_u-2}, Bu.y = B_{my_u-1}, Bu.z = B_{my_u}
    QVector3D Bv;
    QVector3D Bu;
    // bruker my_u pa u-skjotene og my_v pa v-skjotene
    float w_my_1 = (tu-u[my_u] ) / (u[my_u+1] - u[my_u ]);
    float w_my_2 = (tu-u[my_u-1]) / (u[my_u+1] - u[my_u-1]);
    Bv.setX((1-w_my_1)*(1-w_my_2));
    Bv.setY((1-w_my_1)*w_my_2 + w_my_1*(1-w_my_2));
    Bv.setZ(w_my_1*w_my_2);
    w_my_1 = (tv-v [my_v] ) / (v[my_v+1] - v[my_v ]);
    w_my_2 = (tv-v [my_v-1]) / (v[my_v+1] - v[my_v-1]);
    Bu.setX((1-w_my_1)*(1-w_my_2));
    Bu.setY((1-w_my_1)*w_my_2 + w_my_1*(1-w_my_2));
    Bu.setZ(w_my_1*w_my_2);
    return std::pair<QVector3D, QVector3D >(Bu, Bv);
}

//QVector3D TPSurface::evaluate(float tu, float tv, int my_u, int my_v, QVector3D bu, QVector3D bv)
//{

//}
