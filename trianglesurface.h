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
    void origoFixer();
    void init(GLint matrixUniform) override;
    void draw() override;
    std::vector<Vertex>& get_vertices() { return mVertices; } // 191120
    QVector3D grid[12][17];
    QVector3D v0, v1, v2, v3, v4, v5;
    QVector3D n0, n1, n2, n3;
    float xmin = 800000.f, xmax = 0, ymin = 131400000.f, ymax = 0, zmin = 300, zmax = 0;
    float height = 0, depth = 0, width = 0;
};

#endif // TRIANGLESURFACE_H
