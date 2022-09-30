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

    for (unsigned int i = 0; i < mVertices.size(); i++)
    {
        int x = mVertices[i].getX()/10;
        int y = mVertices[i].getY()/10;
        grid[x][y].setZ(mVertices[i].getZ());
    }

    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            grid[i][j].setX(i);
            grid[i][j].setY(j);
            qDebug() << grid[i][j];
        }
    }

//    for(unsigned int i = 0; i < mVertices.size(); i++)
//    {
//        for(unsigned int j = 0; j < mVertices.size(); j++)
//        {

//        }
//    }

    mMatrix.setToIdentity();
}

TriangleSurface::~TriangleSurface()
{

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
             if(vertex.getX() < xmin)
                 xmin = vertex.getX();
             if(vertex.getY() < ymin)
                 ymin = vertex.getY();
             if(vertex.getZ() < zmin)
                 zmin = vertex.getZ();
             if(vertex.getX() > xmax)
                 xmax = vertex.getX();
             if(vertex.getY() > ymax)
                 ymax = vertex.getY();
             if(vertex.getZ() > zmax)
                 zmax = vertex.getZ();
        }
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

void TriangleSurface::origoFixer()
{
    width = xmax - xmin, depth = ymax - ymin, height = zmax - zmin;
    for (unsigned int i = 0; i < mVertices.size(); i++)
    {
        mVertices.at(i).setXYZ(mVertices.at(i).getX()-xmin,
                               mVertices.at(i).getY()-ymin,
                               mVertices.at(i).getZ()-zmin);
        //mVertices.at(i).setNormal(QVector3D(0,0,0));
//        qDebug() << mVertices.at(i).getX() << mVertices.at(i).getY() << mVertices.at(i).getZ();
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
//    initializeOpenGLFunctions();
//    glBindVertexArray( mVAO );
//    // GL_FALSE for QMatrix4x4
//    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
//    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
//    glDrawElements(GL_TRIANGLES, GLsizei(mIndices.size()), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));

    //GLDrawArrays for testing purposes
    glBindVertexArray(mVAO);
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glPointSize(10.0f);
    glDrawArrays(GL_POINTS, 0, mVertices.size());
}

