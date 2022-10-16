#ifndef BSPLINECURVE_H
#define BSPLINECURVE_H

#include <visualobject.h>

class BSplineCurve : public VisualObject
{
public:
    BSplineCurve(int degree, std::vector<QVector3D> points);
    int findKnotInterval(float x);
    QVector3D evaluateBSplineSample(float x);
    void construct();
    void init(GLint matrixUniform) override;
    void draw() override;
private:
    std::vector<double> t;
    std::vector<QVector3D> c;
    int d = 0;
    int n = 0;
};

#endif // BSPLINECURVE_H
