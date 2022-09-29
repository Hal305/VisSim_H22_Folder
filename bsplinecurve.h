#ifndef BSPLINECURVE_H
#define BSPLINECURVE_H

#include <visualobject.h>

class BSplineCurve
{
public:
    BSplineCurve(int degree, std::vector<int> vec, std::vector<QVector2D> points);
    int findKnotInterval(float x);
    QVector3D evaluateBSplineSample(float x);
private:
    std::vector<int> t;
    std::vector<QVector2D> c;
    int d = 0;
    int n = 0;
};

#endif // BSPLINECURVE_H
