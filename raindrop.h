#ifndef RAINDROP_H
#define RAINDROP_H

#include "rollingball.h"

class RainDrop : public RollingBall
{
public:
    RainDrop(int n, float x, float y);
    ~RainDrop();
    void init(GLint matrixUniform) ;
    void draw();
    void spawn(float x, float y);
    void move(float dt) ;
    void setSurface(VisualObject* surface) { triangle_surface = surface; }
    float getZ();
protected:
    VisualObject* triangle_surface;
private:
    float mx = 0, my = 0, mz = 5;
};

#endif // RAINDROP_H
