#include "raindrop.h"

RainDrop::RainDrop(int n, float x, float y) : RollingBall (n)
{
    spawn(x,y);
}

RainDrop::~RainDrop()
{

}

void RainDrop::spawn(float x, float y)
{
    setScale(0.01f);
    mx = x, my = y;
    mPosition.translate(mx, my, mz);
    mMatrix = mPosition*mScale;;
}

void RainDrop::move(float dt)
{
    float G = dt * g;
    mz += G;
    mPosition.translate(0, 0, G);
    mMatrix = mPosition*mScale;
    //qDebug() << G << mz;
}

float RainDrop::getZ()
{
    return mz;
}

void RainDrop::init(GLint matrixUniform)
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

void RainDrop::draw()
{
    glBindVertexArray(mVAO);
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

}
