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
    float mx = 0.5, my = 3, mz = 0;
    float mass = 1;
    void fall();
    void roll();
    float radius = 1; // Ball is 2 vertex positions wide
    float scale = 0.1;
    QVector3D bVector = {0,0,0};
    QVector3D bPrevious = {0,0,g};
    QVector3D tNormal {0,0,0};
    QVector3D oldNormal {0,0,0};
    QVector3D bSpeed {0,0,0};
    int index{0};
    int oldIndex{0};
};
#endif // ROLLINGBALL_H
