#ifndef RAINDROP_H
#define RAINDROP_H

#include "octaball.h"

class RainDrop : public OctaBall
{
public:
    RainDrop(int n, float x, float y);
    ~RainDrop();
    void init(GLint matrixUniform) ;
    void draw();
    void spawn(float x, float y);
    void move(float dt) ;
    void setSurface(VisualObject* surface) { triangle_surface = surface; }
protected:
    VisualObject* triangle_surface;
private:
    float mz;
};

#endif // RAINDROP_H
