#include <iostream>
#include <sstream>
#include "octaball.h"

OctaBall::OctaBall(int n) : VisualObject(), mRecursions(n)
{
    mVertices.reserve(3 * 8 * pow(4, mRecursions));
    octaUnitBall();
    mMatrix.setToIdentity();
}

OctaBall::~OctaBall()
{

}


void OctaBall::makeTriangle(const QVector3D &v1, const QVector3D &v2, const QVector3D &v3)
{
    Vertex v{v1.x(), v1.y(), v1.z(), v1.x(), v1.y(), v1.z()};
    mVertices.push_back(v);
    v = Vertex{v2.x(), v2.y(), v2.z(), v2.x(), v2.y(), v2.z()};
    mVertices.push_back(v);
    v = Vertex{v3.x(), v3.y(), v3.z(), v3.x(), v3.y(), v3.z()};
    mVertices.push_back(v);
}

void OctaBall::subDivide(const QVector3D &a, const QVector3D &b, const QVector3D &c, int n)
{
   if (n>0) {
       QVector3D v1 = a+b; v1.normalize();
       QVector3D v2 = a+c; v2.normalize();
       QVector3D v3 = c+b; v3.normalize();
       subDivide(a, v1, v2, n-1);
       subDivide(c, v2, v3, n-1);
       subDivide(b, v3, v1, n-1);
       subDivide(v3, v2, v1, n-1);
   } else {
       makeTriangle(a, b, c);

   }
}

void OctaBall::octaUnitBall()
{
    QVector3D v0{0, 0, 1};
    QVector3D v1{1, 0, 0};
    QVector3D v2{0, 1, 0};
    QVector3D v3{-1, 0, 0};
    QVector3D v4{0, -1, 0};
    QVector3D v5{0, 0, -1};

    subDivide(v0, v1, v2, mRecursions);
    subDivide(v0, v2, v3, mRecursions);
    subDivide(v0, v3, v4, mRecursions);
    subDivide(v0, v4, v1, mRecursions);
    subDivide(v5, v2, v1, mRecursions);
    subDivide(v5, v3, v2, mRecursions);
    subDivide(v5, v4, v3, mRecursions);
    subDivide(v5, v1, v4, mRecursions);
}

void OctaBall::move(float x, float y, float z)
{
    //mPosition.setToIdentity(); // hvis nødvendig
    QVector4D pos{x,y,z,1.0};
    mPosition.setRow(3,pos);
    mScale.scale(QVector3D(0.25, 0.25, 0.25));
    mMatrix = mScale*mPosition; // * mScale * mRotation etter behov, pass på rekkefølgen
}

void OctaBall::move(float dt)
{
    double degrees = (180 * dt)/M_PI;
    double velocity = dt/2;
    mRotation.rotate(degrees,0.1,0,0);
    mPosition.translate(0,-velocity,0);
    mMatrix = mPosition * mRotation;
}

void OctaBall::init(GLint matrixUniform)
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

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void OctaBall::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());//mVertices.size());
}
