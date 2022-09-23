#include "raindrop.h"

RainDrop::RainDrop(int n, float x, float y) : OctaBall (n)
{
    spawn(x,y);
}

RainDrop::~RainDrop()
{

}

void RainDrop::spawn(float x, float y)
{
    QVector4D pos{x,y,mz,1.0};
    mPosition.setRow(3,pos);
    setScale(0.001f);
    mMatrix = mScale*mPosition;
}

void RainDrop::move(float dt)
{
    float G = dt * g;
    mz += G;
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
