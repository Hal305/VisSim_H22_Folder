#ifndef BARYCENTRICCALC_H
#define BARYCENTRICCALC_H
#include <QVector3D>

class BarycentricCalc
{
public:
    BarycentricCalc(QVector2D point);
    QVector3D calculate(const QVector2D& p1, const QVector2D& p2, const QVector2D& p3);
private:
    QVector2D mPoint;
};

#endif // BARYCENTRICCALC_H
