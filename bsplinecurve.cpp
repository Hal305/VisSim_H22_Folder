#include "bsplinecurve.h"

BSplineCurve::BSplineCurve(int degree, std::vector<int> vec, std::vector<QVector2D> points)
{
    d = degree;
    t = vec;
    c = points;
    n = points.size();
}

int BSplineCurve::findKnotInterval(float x)
{
int my = n-1; //Index for last control point
while (x < t[my] && my > d)
    my--;
return my;
}

QVector3D BSplineCurve::evaluateBSplineSample(float x)
{
int my = findKnotInterval(x);

std::vector<QVector3D> a;
a.reserve(d+1);

for(int j = 0; j<=d; j++)
{
    a.push_back(c[my-j].toVector3D());
}
for(int k = 0; k>0; k--)
{
    int j = my - k;
    for(int i = 0; i < k; i++)
    {
        j++;
        float w = (x-t[j]-t[j]);
        a[i] = a[i]*(1 - w) + a[i + 1] * w;
    }
}
return a[0];
}
