#include "light.h"

Light::Light()
{
     mVertices.push_back(Vertex{-0.25, -0.25, 0.25,     0.8f, 0.8f, 0.3f,   0, 0});
     mVertices.push_back(Vertex{0.25, -0.25, 0.25,      0.8f, 0.8f, 0.3f,   1.f, 0});
     mVertices.push_back(Vertex{0, 0.25, 0,             0.8f, 0.8f, 0.3f,   0.5,0.5});
     mVertices.push_back(Vertex{0, -0.25, -0.25,        0.8f, 0.8f, 0.3f,   0.5,0.5});

    mIndices =
    { 0, 1, 2,
      1, 3, 2,
      3, 0, 2,
      0, 3, 1
    };

    mMatrix.setToIdentity();
}

Light::Light(GLuint ShaderId)
{
    mShaderId=ShaderId;
    mVertices.push_back(Vertex{-0.25,-0.25,0.25,0.8f,0.8f,0.3f,0,0});
    mVertices.push_back(Vertex{0.25,-0.25,0.25,0.8f,0.8f,0.3f,1.f,0});
    mVertices.push_back(Vertex{0,0.25,0,0.8f,0.8f,0.3f,0.5,0.5});
    mVertices.push_back(Vertex{0,-0.25,-0.25,0.8f,0.8f,0.3f,0.5,0.5});

    mIndices =
    { 0, 1, 2,
      1, 3, 2,
      3, 0, 2,
      0, 3, 1
    };

    mMatrix.setToIdentity();
}

void Light::init(GLint MatrixUniform)
{
    mMatrixUniform = MatrixUniform;
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
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof( Vertex ), (GLvoid*)( 6 * sizeof( GLfloat ) ));
    glEnableVertexAttribArray(2);

    //Second buffer - holds the indices (Element Array Buffer - EAB):
    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Light::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
