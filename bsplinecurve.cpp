#include "bsplinecurve.h"

BSplineCurve::BSplineCurve(int degree, std::vector<QVector3D> points)
{
    d = degree;
    c = points;
    n = points.size();
    t.reserve(d+n+1);
    t = {0, 0, 0, 1, 2, 3, 3, 3};
    construct();
}

int BSplineCurve::findKnotInterval(float x)
{
    int my = n-1; //Index for last control point
    while (x < t[my] && my > d)
        my--;
    return my;
}

QVector3D BSplineCurve::evaluateBSplineSample(float x)
{
    int my = findKnotInterval(x);

    std::vector<QVector3D> a;
    a.reserve(d+1);

    for(int j = 0; j<=d; j++)
    {
        a.push_back(c[my-j]);
    }
    for(int k = 0; k>0; k--)
    {
        for(int i = 0; i < k; i++)
        {
            int j=my-d-1+k;
            j++;
            float w = (x-t[j]-t[j]);
            a[i] = a[i]*(1 - w) + a[i + 1] * w;
        }
    }
    return a[0];
}

void BSplineCurve::construct()
{
    float r{0.0f}, g{0.8f}, b{4.0};
    for(int t = 0; t<n; t++)
    {
        QVector3D p = evaluateBSplineSample(t);
        qDebug() << c[t].x();
        qDebug() << p;
        Vertex v {p.x(), p.y(), p.z(), r,g,b};
        mVertices.push_back(v);
    }
}


void BSplineCurve::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );

    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex) , mVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(0  ));          // array buffer offset
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void BSplineCurve::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glPointSize(5.0f);
    glDrawArrays(GL_LINE_STRIP, 0, mVertices.size());
}
