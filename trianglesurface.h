#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include <visualobject.h>
#include <barycentriccalc.h>

class TriangleSurface : public VisualObject
{
public:
    TriangleSurface();
    TriangleSurface(std::string filename);
    ~TriangleSurface() override;
    void readFile(std::string filename);
    void writeFile(std::string filename);
    void origoFixer();
    float heightCalc(float x, float y);
    void init(GLint matrixUniform) override;
    void draw() override;
    std::vector<Vertex>& get_vertices() { return mVertices; } // 191120
    QVector3D grid[12][17];
    float xmin = 800000.f, xmax = 0, ymin = 131400000.f, ymax = 0, zmin = 300, zmax = 0;
    float width = 0, depth = 0, height = 0;
};

#endif // TRIANGLESURFACE_H
