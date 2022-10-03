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
    Vertex::Triangle findTriangle(float x, float y);
    QVector3D old_normal{0.0, 0.0, 1.0};
    int old_index{0};
    float mx = 0, my = 0, mz = 0.15f;
    float dx = 0, dy = 0, dz = 0;
    float lastz = 0;
    float mP = 0, mQ = 0, mR = 0;
    QVector2D P{0,0}, Q{0,0}, R{0,0};
    QVector3D normal = {0,0,0};
};
#endif // ROLLINGBALL_H
