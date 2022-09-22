#ifndef OCTABALL_H
#define OCTABALL_H

#include "visualobject.h"

class OctaBall : public VisualObject
{
public:
    OctaBall(int n);
    ~OctaBall() override;
    void init(GLint matrixUniform) override;
    void draw() override;

private:
    int mRecursions;
    int mIndex; // Used in recursion, to build m_vertices
    std::vector<GLuint> mIndices;
    GLuint mIBO{0};
    void makeTriangle(const QVector3D& v1, const QVector3D& v2, const QVector3D& v3);
    void subDivide(const QVector3D& a, const QVector3D& b, const QVector3D& c, int n);
    void octaUnitBall();
    void move(float x, float y, float z) override;
    void move(float dt) override;
};

#endif // OCTABALL_H
