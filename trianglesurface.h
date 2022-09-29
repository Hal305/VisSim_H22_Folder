#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include <visualobject.h>

class TriangleSurface : public VisualObject
{
public:
    TriangleSurface();
    TriangleSurface(std::string filename);
    ~TriangleSurface() override;
    void readFile(std::string filename);
    void writeFile(std::string filename);
    void init(GLint matrixUniform) override;
    void draw() override;
    void origoFixer();
    std::vector<Vertex>& get_vertices() { return mVertices; } // 191120
    QVector3D v0, v1, v2, v3, v4, v5;
    QVector3D n0, n1, n2, n3;
    float xmin = 800000.f, xmax = 0, ymin = 131400000.f, ymax = 0;
};

#endif // TRIANGLESURFACE_H
