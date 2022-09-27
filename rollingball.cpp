#include "rollingball.h"

RollingBall::RollingBall(int n) : OctaBall (n)
{
    //mVelocity = QVector3d{1.0f, 1.0f, -0.05f};
    mPosition.translate(mx,my,mz);
    setScale(0.1);
    mMatrix = mPosition * mScale;
}

RollingBall::~RollingBall()
{

}

void RollingBall::move(float dt)
{
    // Finne trekant
    float mP = 0;
    float mQ = 0;
    float mR = 0;
    QVector2D P{0,0}, Q{0,0}, R{0,0};
    std::vector<Vertex>& vertices = dynamic_cast<TriangleSurface*>(triangle_surface)->get_vertices();

    //    qDebug() << vertices[0].getX() << vertices[0].getY() << vertices[0].getZ();
    //    qDebug() << vertices[1].getX() << vertices[1].getY() << vertices[1].getZ();
    //    qDebug() << vertices[2].getX() << vertices[2].getY() << vertices[2].getZ();
    //    qDebug() << vertices[3].getX() << vertices[3].getY() << vertices[3].getZ();
    //    qDebug() << vertices[4].getX() << vertices[4].getY() << vertices[4].getZ();
    //    qDebug() << vertices[5].getX() << vertices[5].getY() << vertices[5].getZ();

    mx+=dt, my -= dt*0.66f;
    BarycentricCalc BC(QVector2D(mx, my));

    P.setX(vertices[0].getX()), P.setY(vertices[0].getY());
    Q.setX(vertices[1].getX()), Q.setY(vertices[1].getY());
    R.setX(vertices[4].getX()), R.setY(vertices[4].getY());
    QVector3D baryc = BC.calculate(P,Q,R);
    float u = baryc.x(), v = baryc.y(), w = baryc.z();
    mP = u * vertices[0].getZ();
    mQ = v * vertices[1].getZ();
    mR = w * vertices[4].getZ();
    if(u >= 0 && v >= 0 && w >= 0
            && u <= 1 && v <= 1 && w<= 1)
    {
        mz = (mP + mQ + mR);
        QVector3D n0 = {mVertices[0].getX(), mVertices[0].getY(), mVertices[0].getZ()};
        mPosition.translate(dt, -dt * 0.66f, mz - lastz);
        mMatrix = mPosition*mScale;
        lastz = mz;
//        QVector3D v0 = {mVertices[0].getX(), mVertices[0].getY(), mVertices[0].getZ()};
//        QVector3D v1 = {mVertices[1].getX(), mVertices[1].getY(), mVertices[1].getZ()};
//        QVector3D v2 = {mVertices[4].getX(), mVertices[4].getY(), mVertices[4].getZ()};
//        normal = QVector3D::normal(v0, v1, v2);
//        mMatrix = mPosition * mScale;
//        mPosition.translate(normal.x(), normal.y(), normal.z()+g);
//        qDebug() << normal.z()+g;
    }
    else
    {
        //qDebug() << "Wrong triangle";
        P.setX(vertices[3].getX()), P.setY(vertices[3].getY());
        Q.setX(vertices[0].getX()), Q.setY(vertices[0].getY());
        R.setX(vertices[4].getX()), R.setY(vertices[4].getY());
        baryc = BC.calculate(P,Q,R);
        u = baryc.x(), v = baryc.y(), w = baryc.z();
        mP = u * vertices[3].getZ();
        mQ = v * vertices[0].getZ();
        mR = w * vertices[4].getZ();
        if(u >= 0 && v >= 0 && w >= 0
                && u <= 1 && v <= 1 && w<= 1)
        {
            mz = (mP + mQ + mR);
            mPosition.translate(dt, -dt * 0.66f, mz - lastz);
            mMatrix = mPosition*mScale;
            lastz = mz;
        }
        else
        {
            //qDebug() << "Wrong triangle";
            P.setX(vertices[1].getX()), P.setY(vertices[1].getY());
            Q.setX(vertices[2].getX()), Q.setY(vertices[2].getY());
            R.setX(vertices[4].getX()), R.setY(vertices[4].getY());
            baryc = BC.calculate(P,Q,R);
            u = baryc.x(), v = baryc.y(), w = baryc.z();
            mP = u * vertices[1].getZ();
            mQ = v * vertices[2].getZ();
            mR = w * vertices[4].getZ();
            if(u >= 0 && v >= 0 && w >= 0
                    && u <= 1 && v <= 1 && w<= 1)
            {
                mz = (mP + mQ + mR);
                mPosition.translate(dt, -dt * 0.66f, mz - lastz);
                mMatrix = mPosition*mScale;
                lastz = mz;
            }
            else
            {
                //qDebug() << "Wrong triangle";
                P.setX(vertices[2].getX()), P.setY(vertices[2].getY());
                Q.setX(vertices[5].getX()), Q.setY(vertices[5].getY());
                R.setX(vertices[4].getX()), R.setY(vertices[4].getY());
                baryc = BC.calculate(P,Q,R);
                u = baryc.x(), v = baryc.y(), w = baryc.z();
                mP = u * vertices[2].getZ();
                mQ = v * vertices[5].getZ();
                mR = w * vertices[4].getZ();
                if(u >= 0 && v >= 0 && w >= 0
                        && u <= 1 && v <= 1 && w<= 1)
                {
                    mz = (mP + mQ + mR);
                    mPosition.translate(dt, -dt * 0.66f, mz - lastz);
                    mMatrix = mPosition*mScale;
                    lastz = mz;
                }
                else
                {
//                    qDebug() << "Off";
                    mz = 0;
                    mPosition.translate(dt, -dt * 0.66f, mz - lastz);
                    mMatrix = mPosition*mScale;
                    lastz = mz;
                }
            }
        }
    }
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
