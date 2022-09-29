#include "vertex.h"

Vertex::Vertex()
{

}

Vertex::Vertex(float x, float y, float z, float r, float g, float b)
    :   m_xyz{x, y, z}, m_rgb{r, g, b}
{

}

Vertex::Vertex(float x, float y, float z, float r, float g, float b, float u, float v)
    :   m_xyz{x, y, z}, m_rgb{r, g, b}, m_uv{u, v}
{

}

Vertex::Vertex(QVector3D a, QVector3D b, QVector2D c)
{
    m_xyz[0]=a.x();
    m_xyz[1]=a.y();
    m_xyz[2]=a.z();
    m_rgb[0]=b.x();
    m_rgb[1]=b.y();
    m_rgb[2]=b.z();
    m_uv[0]=c.x();
    m_uv[1]=c.y();
}

float Vertex::getX()
{
    return m_xyz[0];
}

float Vertex::getY()
{
    return m_xyz[1];
}

float Vertex::getZ()
{
    return m_xyz[2];
}

void Vertex::setXYZ(float x, float y, float z)
{
    m_xyz[0] = x;
    m_xyz[1] = y;
    m_xyz[2] = z;
}

void Vertex::setNormal(QVector3D normal)
{
    m_rgb[0] = normal.x();
    m_rgb[1] = normal.y();
    m_rgb[2] = normal.z();
}

std::ostream& operator<< (std::ostream& os, const Vertex& v) {
  os << std::fixed;
  os << "(" << v.m_xyz[0] << ", " << v.m_xyz[1] << ", " << v.m_xyz[2] << ") ";
  os << "(" << v.m_rgb[0] << ", " << v.m_rgb[1] << ", " << v.m_rgb[2] << ") ";
  os << "(" << v.m_uv[0] << ", " << v.m_uv[1] << ") ";
  return os;
}

std::istream& operator>> (std::istream& is, Vertex& v) {
// Trenger fire temporære variabler som kun skal lese inn parenteser og komma
  char dum, dum2, dum3, dum4;
  is >> dum >> v.m_xyz[0] >> dum2 >> v.m_xyz[1] >> dum3 >> v.m_xyz[2] >> dum4;
  is >> dum >> v.m_rgb[0] >> dum2 >> v.m_rgb[1] >> dum3 >> v.m_rgb[2] >> dum4;
  is >> dum >> v.m_uv[0] >> dum2 >> v.m_uv[1] >> dum3;
  return is;
}

