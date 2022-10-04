#include "rollingball.h"

RollingBall::RollingBall(int n) : OctaBall (n)
{
    //mVelocity = QVector3d{1.0f, 1.0f, -0.05f};
    //findTriangle(0,0);
    mPosition.translate(mx,my,mz);
    setScale(0.1);
    mMatrix = mPosition * mScale;
}

RollingBall::~RollingBall()
{

}

void RollingBall::move(float dt)
{
    Vertex::Triangle currentTriangle(0,0,0,0,0,0);
    float dx = dt, dy = -dt * 0.66f, dz = g*dt;
    mx+=dx, my -= dy*0.66f;
    currentTriangle = dynamic_cast<TriangleSurface*>(triangle_surface)->findTriangle(mx,my);
    mz = dynamic_cast<TriangleSurface*>(triangle_surface)->zReturn;

    mPosition.translate(dx, dy, mz-lastz);
    mMatrix = mPosition*mScale;
    //qDebug() << mz;
    lastz = mz;
    {
        // beregne normal
        QVector3D normal = dynamic_cast<TriangleSurface*>(triangle_surface)->normalize(currentTriangle);
        qDebug() << normal;
        // beregn akselerasjonsvektor−ligning(7)
        // Oppdaterer hastighet og posisjon
        //if ( /* ny indeks != forrige indeks */)
        {
            // Ball en har rullet over på nytt triangel
            // Beregner normalen til lkollisjonsplanet,
            // se ligning (9)
            // Korrigereposisjon oppover i normalens retning
            // Oppdater hastighetsvektoren, se ligning (8)
            // Oppdatere posisjon i retning den nye
            // hastighetsvektoren
        }
        // Oppdater gammel normal og indeks
    }
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
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

}
