#ifndef TPSURFACE_H
#define TPSURFACE_H

#include "trianglesurface.h"

class TPSurface : public TriangleSurface
{
private:
    float u [9] {0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 4.0f, 4.0f};
    float v [8] {0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 3.0f, 3.0f, 3.0f};
    // Gjor om til Vector3D matrise
    float c [9][8];
    int n_u{4}; // antall kontrollpunkter for skjotvektor u
    int n_v{3}; // antall kontrollpunkter for skjotvektor v
    int d {2};  // grad (samme i begge retninger)
    std::pair<int,int> findMy (float tu, float tv); // midlertidig
    std::pair<QVector3D, QVector3D> B2 (float tu, float tv, int my_u, int my_v);
    QVector3D evaluate(float tu, float tv, int my_u, int my_v, QVector3D bu, QVector3D bv);
public:
    TPSurface();
};

#endif // TPSURFACE_H
