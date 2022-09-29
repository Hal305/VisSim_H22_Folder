#include <QDebug>
#include <QImage>
#include <cstdio>
#include <iostream>
#include "trianglesurface.h"
#include "vertex.h"

TriangleSurface::TriangleSurface() : VisualObject()
{

}

TriangleSurface::TriangleSurface(std::string filename) : VisualObject()
{
    readFile(filename);

    origoFixer();

//    mIndices = {0,4,3,
//                0,1,4,
//                1,2,4,
//                2,5,4
//               };

    //mMatrix.translate(-xmin, -ymin, -220);

//    v0 = {mVertices[0].getX(), mVertices[0].getY(), mVertices[0].getZ()},
//    v1 = {mVertices[1].getX(), mVertices[1].getY(), mVertices[1].getZ()},
//    v2 = {mVertices[2].getX(), mVertices[2].getY(), mVertices[2].getZ()},
//    v3 = {mVertices[3].getX(), mVertices[3].getY(), mVertices[3].getZ()},
//    v4 = {mVertices[4].getX(), mVertices[4].getY(), mVertices[4].getZ()},
//    v5 = {mVertices[5].getX(), mVertices[5].getY(), mVertices[5].getZ()};

//    n0 = QVector3D::normal(v0, v4, v3),
//    n1 = QVector3D::normal(v0, v1, v4),
//    n2 = QVector3D::normal(v1, v2, v4),
//    n3 = QVector3D::normal(v2, v5, v4);

    //qDebug() << n0.x() << n0.y() << n0.z();

    mMatrix.setToIdentity();
    //mMatrix.translate(0,0,5);
}

TriangleSurface::~TriangleSurface()
{
    //qDebug() << "TriangleSurface::~TriangleSurface()";
   //delete [] m_vertices;
    //qDebug() << "TriangleSurface::~TriangleSurface() - vertices deleted";
}

void TriangleSurface::readFile(std::string filename)
{
    std::ifstream inn;
    inn.open(filename.c_str());

    if (inn.is_open())
    {
        int n;
        Vertex vertex;
        inn >> n;
        mVertices.reserve(n);
        for (int i=0; i<n; i++)
        {
             inn >> vertex;
             mVertices.push_back(vertex);
             //qDebug() << "x = " << vertex.getX() <<  "y = " << vertex.getY()<< "z = " << vertex.getZ();
             if(vertex.getX() < xmin)
                 xmin = vertex.getX();
             if(vertex.getY() < ymin)
                 ymin = vertex.getY();
             if(vertex.getX() > xmax)
                 xmax = vertex.getX();
             if(vertex.getY() > ymax)
                 ymax = vertex.getY();
        }
        qDebug() << xmin << ymin;
        qDebug() << xmax << ymax;
        qDebug() << xmax - xmin << ymax - ymin;

//        inn >> n;
//        mIndices.reserve(n);
//        for(int i=0; i<n; i++)
//        {
//            mIndices.push_back(i);
//        }
        inn.close();
    }
}

void TriangleSurface::writeFile(std::string filename)
{
    std::ofstream ut;
    ut.open(filename.c_str());

    if (ut.is_open())
    {

        auto n = mVertices.size();
        Vertex vertex;
        ut << n << std::endl;
        for (auto it=mVertices.begin(); it != mVertices.end(); it++)
        {
            //vertex = *it;
            ut << *it << std::endl;
        }
        ut.close();
    }
}

void TriangleSurface::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), reinterpret_cast<const void*>(0));
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void TriangleSurface::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    // GL_FALSE for QMatrix4x4
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glDrawElements(GL_TRIANGLES, GLsizei(mIndices.size()), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
}

void TriangleSurface::origoFixer()
{

}
