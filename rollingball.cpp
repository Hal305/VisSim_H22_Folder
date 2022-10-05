#include "rollingball.h"

RollingBall::RollingBall(int n) : OctaBall (n)
{
    //mVelocity = QVector3d{1.0f, 1.0f, -0.05f};
    //findTriangle(0,0);
    mPosition.translate(mx,my,mz);
    radius *= scale;
    setScale(scale);
    mMatrix = mPosition * mScale;
}

RollingBall::~RollingBall()
{

}

void RollingBall::move(float dt)
{
    //qDebug() << "Old normal ="<< tNormal;
    QVector3D P0 = {mx, my, mz};
    Vertex::Triangle currentTriangle(0,0,0,0,0,0);
    currentTriangle = dynamic_cast<TriangleSurface*>(triangle_surface)->findTriangle(mx,my,mz);
    //mz = dynamic_cast<TriangleSurface*>(triangle_surface)->zReturn;
    int index = dynamic_cast<TriangleSurface*>(triangle_surface)->Ti;

    if(dynamic_cast<TriangleSurface*>(triangle_surface)->OOB == false)
    {
        qDebug() << "Current normal ="<< tNormal;
        // Calculate normal
        tNormal = dynamic_cast<TriangleSurface*>(triangle_surface)->normalize(currentTriangle);

        // Calculate acceleration vector, equation 8.7
        bVector = {tNormal.x()*tNormal.z(), tNormal.y()*tNormal.z(), (tNormal.z()*tNormal.z())-1};
        bVector.setZ(bVector.z()*g*dt);

        //mx += bVector.x(), my += bVector.y(), mz += bVector.z();
        //bVector.setZ(bVector.z()+g);

        // Update velocity and position
        if(oldIndex!=index)
        {
            QVector3D P = {mx, my, mz};
            QVector3D y = (P-P0) * tNormal;
            // The ball has rolled onto a new triangle
            // Calculate the normal for the collision plane
            // bVector = oldNormal - 2 * (oldNormal * tNormal) * tNormal;
            QVector3D l = (y*tNormal)/y.length();
            float i = l.x() + l.y() + l.z();
            //qDebug() << i;
            if(i < 0)
            {
                // Correct the position upwards in the normal's direction, equation 8.9
                bVector *= radius - y.length(); //Adjusting for offset by D = r - y
                mx += bVector.x(), my += bVector.y(), mz += bVector.z();
            }
            // Update the velocity vector, equation 8.8
            // previous v = v - 2 * (v * n) * n
            bVector = bPrevious - 2 * (bPrevious * tNormal) * tNormal;
        }
    }
    else
        bVector.setZ((bVector.z()+0.5)*g*dt);

    //qDebug() << bVector.z();
    // Update position in the direction of the new velocity vector
    bSpeed += bVector*dt;
    mx += bSpeed.x(), my += bSpeed.y(), mz += bSpeed.z();
    // Update old normal and index
    mPosition.setColumn(3, QVector4D(mx, my, mz, 1));
    mMatrix = mPosition*mScale;
    bPrevious = bVector;
    oldNormal = tNormal;
    oldIndex = index;
}

void RollingBall::fall()
{

}

void RollingBall::roll()
{

}

void RollingBall::init(GLint matrixUniform)
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

void RollingBall::draw()
{
    glBindVertexArray(mVAO);
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, GLsizei(mVertices.size()));
}
