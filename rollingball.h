#ifndef ROLLINGBALL_H
#define ROLLINGBALL_H

#include "octaball.h"
#include "trianglesurface.h"
#include "barycentriccalc.h"

class RollingBall : public OctaBall
{
public:
    RollingBall(int n);
    ~RollingBall() ;
    void init(GLint matrixUniform) ;
    void draw();
    void move(float dt) ;
    void setSurface(VisualObject* surface) { triangle_surface = surface; }
protected:
    VisualObject* triangle_surface;
private:
    float mx = 0, my = 0, mz = 0;
    float dx = 0, dy = 0, dz = 0;
    float mP = 0, mQ = 0, mR = 0;
    QVector2D P{0,0}, Q{0,0}, R{0,0};
    QVector3D bVector = {0,0,0};
    QVector3D oldNormal {0,0,1};
    int oldIndex{0};
};
#endif // ROLLINGBALL_H
