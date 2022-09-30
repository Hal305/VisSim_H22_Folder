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


    mVertices.clear();
    depth = 17, width = 12;
    float c;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < depth; j++)
        {
            grid[i][j].setX(i);
            grid[i][j].setY(j);
            float x = (grid[i][j].x() -width/2)*2;
            float y = (grid[i][j].y() -depth/2)*2;
            float z = (grid[i][j].z()/4 -10);
            c = grid[i][j].z()/100;
            Vertex v = {x,y,z, c,c,c, 0,0};
            mVertices.push_back(v);
            qDebug() << grid[i][j];
        }
    }

    for(int i = 0; i < width - 1; i++)       // for each row a.k.a. each strip
    {
        for(int j = 0; j < depth - 1; j++)      // for each column
        {
            int Vi = (i * depth) + j;
            mIndices.push_back(Vi);
            mIndices.push_back(Vi + depth);
            mIndices.push_back(Vi + 1);

            mIndices.push_back(Vi + depth +1);
            mIndices.push_back(Vi + 1);
            mIndices.push_back(Vi + depth);
        }
    }

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
    }
}

float TriangleSurface::heightCalc(float x, float y)
{
    QVector2D P = {0,0}, Q = {0,0}, R = {0,0};
    int i = y + depth/2;
    int j = x + width/2;
    float z = 0;
    float Vi = (i * depth) + j;
    if(j>0 && j<width && i>0 && i<depth)
    {
        BarycentricCalc bc(QVector2D{x, y});
        P.setX(mVertices[Vi].getX()), P.setY(mVertices[Vi].getY());
        Q.setX(mVertices[Vi+width].getX()), Q.setY(mVertices[Vi+width].getY());
        R.setX(mVertices[Vi+1].getX()), R.setY(mVertices[Vi+1].getY());
        if(Q.x()==P.x())
        {
            Q.setX(mVertices[Vi-1].getX()), Q.setY(mVertices[Vi-1].getY());
            R.setX(mVertices[Vi-width].getX()), R.setY(mVertices[Vi-width].getY());
        }
        QVector3D baryc = bc.calculate(P,Q,R);
        float u = baryc.x(), v = baryc.y(), w = baryc.z();
        //f(x,y) = u * f(Px,Py) + v * f(Qx,Qy) + w * f(Rx,Ry)
        float mP = u * mVertices[Vi].getZ();
        float mQ = v * mVertices[Vi+width].getZ();
        float mR = w * mVertices[Vi+1].getZ();
        z = 0.1 + (mP + mQ + mR);
    }
    else
        z = -1000;
    return z;
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

//    //GLDrawArrays for testing purposes
//    glBindVertexArray(mVAO);
//    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
//    glPointSize(10.0f);
//    glDrawArrays(GL_POINTS, 0, mVertices.size());
}

