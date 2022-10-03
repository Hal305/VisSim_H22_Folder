#include "rollingball.h"

RollingBall::RollingBall(int n) : OctaBall (n)
{
    //mVelocity = QVector3d{1.0f, 1.0f, -0.05f};
    currentTriangle;
    mPosition.translate(mx,my,mz);
    setScale(0.1);
    mMatrix = mPosition * mScale;
}

RollingBall::~RollingBall()
{

}

void RollingBall::move(float dt)
{
    mx+=dt, my -= dt*0.66f;
    // Finne trekant
    std::vector<Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    currentTriangle = findTriangle(mx, my);
    float mP = u * vertices[currentTriangle.indexes[0]].getZ();
    float mQ = v * vertices[currentTriangle.indexes[1]].getZ();
    float mR = w * vertices[currentTriangle.indexes[2]].getZ();
    mz = 0.1 + (mP + mQ + mR);

    mPosition.translate(dt, -dt * 0.66f, mz-lastz);
    mMatrix = mPosition*mScale;
    //qDebug() << mz;
    lastz = mz;
    {
        // beregne normal
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

Vertex::Triangle RollingBall::findTriangle(float x, float y)
{
    std::vector<Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();
    std::vector<Vertex::Triangle>& triangles = dynamic_cast<TriangleSurface*>(triangle_surface)->get_triangles();
    int depth = dynamic_cast<TriangleSurface*>(triangle_surface)->depth;
    int width = dynamic_cast<TriangleSurface*>(triangle_surface)->width;
    int mDepth = (y + depth/2)*2;
    int mWidth = (x + width/2)*2;
    int i = (mDepth * depth) + mWidth; // Might have to change these around

    qDebug() << x << y << i << mDepth << mWidth;
    bool found = false;
    do {
        QVector2D P = {vertices[triangles[i].indexes[0]].getX(), vertices[triangles[i].indexes[0]].getY()},
                Q = {vertices[triangles[i].indexes[1]].getX(), vertices[triangles[i].indexes[1]].getY()},
                R = {vertices[triangles[i].indexes[2]].getX(), vertices[triangles[i].indexes[2]].getY()};
    qDebug() << P << Q << R;
    //Beregn barysentriske koordinater for trekant i
        BarycentricCalc BC(QVector2D{x,y});
        QVector3D result = BC.calculate(P, Q, R);
        u = result.x(), v = result.y(), w = result.z();
        qDebug() << i << u << v << w;
    if (u >= 0 && v >= 0 && w >= 0
            && u <= 1 && v <= 1 && w <= 1)
        found = true;
    //else i = nabo som svarer til minste barysentriske koordinaten
    else
        if(u<=v && u<=w && triangles[i].neighbours[0] != -1)
            i = triangles[i].neighbours[0];
        else if(v<=u && v<=w && triangles[i].neighbours[1] != -1)
            i = triangles[i].neighbours[1];
        else if(w<=u && w<=v && triangles[i].neighbours[2] != -1)
            i = triangles[i].neighbours[2];
        else
        {
            qDebug() << "Out of bounds";
            found = true;
        }
    } while (!found);
    qDebug() << "Triangle found!" << triangles[i].indexes[0] << triangles[i].indexes[1] << triangles[i].indexes[2];
    return triangles[i];
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
