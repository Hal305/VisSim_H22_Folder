#include <QDebug>
#include <QImage>
#include <cstdio>
#include <iostream>
#include "trianglesurface.h"
#include "vertex.h"

TriangleSurface::TriangleSurface() : VisualObject()
{

}

TriangleSurface::TriangleSurface(std::string filename, GLuint ShaderId, GLuint TextureId) : VisualObject()
{
    mShaderId = ShaderId;
    mTextureId = TextureId;
    readFile(filename);

    origoFixer();
    construct();
    triangulate();
    normalize();
    //findTriangle(0, 0);

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

void TriangleSurface::construct()
{
    for (unsigned int i = 0; i < mVertices.size(); i++)
    {
        int x = mVertices[i].getX()/10;
        int y = mVertices[i].getY()/10;
        grid[x][y].setZ(mVertices[i].getZ());
    }

    mVertices.clear();
    depth = 17, width = 12;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < depth; j++)
        {
            grid[i][j].setX(i);
            grid[i][j].setY(j);
            float x = (grid[i][j].x() -width/2)*2;
            float y = (grid[i][j].y() -depth/2)*2;
            float z = (grid[i][j].z()/4 -10);
            Vertex v = {x,y,z, 0,0,0, 0,0};
            mVertices.push_back(v);
            //qDebug() << grid[i][j];
        }
    }

    for(int i = 0; i < width - 1; i++)       // for each row a.k.a. each strip
    {
        for(int j = 0; j < depth - 1; j++)      // for each column
        {
            int Vi = (i * depth) + j;
            mIndices.push_back(Vi);         //Lower left
            mIndices.push_back(Vi + depth); //Lower right
            mIndices.push_back(Vi + 1);     //Upper left

            mIndices.push_back(Vi + depth +1);  //Upper right
            mIndices.push_back(Vi + 1);
            mIndices.push_back(Vi + depth);
        }
    }
}

void TriangleSurface::triangulate()
{
    int t = 0;
    int size = (width-1)*(depth-1)*2;
    int yloop = 32; //used to check if we've looped the grid on the y axis
    int n0, n1, n2; //neighbours
    for(unsigned int i = 0; i < mIndices.size(); i+=6) // +1 per triangle, 2 triangles at a time
    {
        //Bottom left triangle, even numbered triangles
        n0 = t + 1;     //the neighbour above
        n1 = t - 31;    //the neighbour to the left
        if(n1 < 0)      //check to see if n1 is outside the grid
            n1 = -1;

        if(yloop == 32) //using yloop to see if we're at the bottom of the grid
        {
            n2 = -1;
            yloop = 0;  //reset yloop
            //qDebug() << "Y loop reset";
        }
        else            //the neighbour below if we're not at the bottom of the grid
            n2 = t - 1;
        Vertex::Triangle tEven(mIndices.at(i), mIndices.at(i + 1), mIndices.at(i + 2), n0, n1, n2);
        mTriangles.push_back(tEven);
        //qDebug() << "t" << t << ": " << mIndices[i] << mIndices[i + 1] << mIndices[i + 2] << n0 << n1 << n2;
        t++;

        //Top right triangle, odd numbered triangles
        n0 = t + 31;    //the neighbour to the right
        if(n0 >= size)
            n0 = -1;

        if(yloop == 30) //using yloop to see if we're at the top of the grid
            n1 = -1;
        else
            n1 = t + 1;     //the neighbour above

        n2 = t - 1;     //the neighbour below
        Vertex::Triangle tOdd(mIndices.at(i + 3), mIndices.at(i + 4), mIndices.at(i + 5), n0, n1, n2);
        mTriangles.push_back(tOdd);
        //qDebug() << "t" << t << ": "<< mIndices[i+3] << mIndices[i + 4] << mIndices[i + 5] << n0 << n1 << n2;
        yloop += 2;     //moving two times up the grid
        t++;
//        qDebug() << tEven.indexes[0] << tEven.indexes[1] << tEven.indexes[2]
//                << tEven.neighbours[0] << tEven.neighbours[1] << tEven.neighbours[2]
//                << "\n" << tOdd.indexes[0] << tOdd.indexes[1] << tOdd.indexes[2]
//                   << tOdd.neighbours[0] << tOdd.neighbours[1] << tOdd.neighbours[2];
    }
}

void TriangleSurface::normalize()
{
    for(int i = 0; i < width-1; i++)
    {
        for(int j = 0; j < depth-1; j++)
        {
            float Vi = i * depth + j; //Current vertex
            int vcount = 0;
            QVector3D n = {0,0,0};
            QVector3D V0, V1, V2, V3, V4, V5, V6 = {0, 0, 1};
            V0 = {Vi, Vi, mVertices[Vi].getZ()};
            if(Vi - width >= 0)
            {   //Lower vertex
                V1 = {Vi, Vi - width, mVertices[Vi - width].getZ()};
                vcount++;
            }
            if(Vi + 1 <= width && Vi - width >= 0)
            {   //Lower right vertex
                V2 = {Vi + 1, Vi - width, mVertices[Vi - width + 1].getZ()};
                vcount++;
            }
            if(Vi + 1 <= width)
            {   //Right vertex
                V3 = {Vi + 1, Vi, mVertices[Vi + 1].getZ()};
                vcount++;
            }
            if(Vi + width <= depth)
            {   //Upper vertex
                V4 = {Vi, Vi + width, mVertices[Vi + width].getZ()};
                vcount++;
            }
            if(Vi + width <= depth && Vi - 1 >= 0)
            {   //Upper left vertex
                V5 = {Vi -1, Vi + width, mVertices[Vi + width - 1].getZ()};
                vcount++;
            }
            if(Vi - 1 >= 0)
            {   //Left vertex
                V6 = {Vi -1, Vi, mVertices[Vi - 1].getZ()};
                vcount++;
            }
            n += QVector3D::normal(V0, V2, V1); //T0
            n += QVector3D::normal(V0, V3, V2); //T1
            n += QVector3D::normal(V0, V4, V3); //T2
            n += QVector3D::normal(V0, V5, V4); //T3
            n += QVector3D::normal(V0, V6, V5); //T4
            n += QVector3D::normal(V0, V6, V1); //T5
            n/=vcount;
            n.normalize();
            mVertices[Vi].setNormal(n);
        }
    }
}

QVector3D TriangleSurface::normalize(Vertex::Triangle t)
{
    QVector3D n0 = {mVertices[t.indexes[0]].getX(),
                    mVertices[t.indexes[0]].getY(),
                    mVertices[t.indexes[0]].getZ()};
    QVector3D n1 = {mVertices[t.indexes[1]].getX(),
                    mVertices[t.indexes[1]].getY(),
                    mVertices[t.indexes[1]].getZ()};
    QVector3D n2 = {mVertices[t.indexes[2]].getX(),
                    mVertices[t.indexes[2]].getY(),
                    mVertices[t.indexes[2]].getZ()};
return QVector3D::normal(n0,n1,n2);
}

Vertex::Triangle TriangleSurface::findTriangle(float x, float y)
{
    qDebug() << x << y << Ti;
    float u =0, v = 0, w = 0;
    bool found = false;
    if(Ti>=0 && Ti < mTriangles.size())
    {
        do {
            QVector2D P = {mVertices[mTriangles[Ti].indexes[0]].getX(), mVertices[mTriangles[Ti].indexes[0]].getY()},
                    Q = {mVertices[mTriangles[Ti].indexes[1]].getX(), mVertices[mTriangles[Ti].indexes[1]].getY()},
                    R = {mVertices[mTriangles[Ti].indexes[2]].getX(), mVertices[mTriangles[Ti].indexes[2]].getY()};
            //qDebug() << P << Q << R;
            //Beregn barysentriske koordinater for trekant i
            BarycentricCalc BC(QVector2D{x,y});
            QVector3D result = BC.calculate(P, Q, R);
            u = result.x(), v = result.y(), w = result.z();
            //qDebug() << i << u << v << w;
            if (u >= 0 && v >= 0 && w >= 0
                    && u <= 1 && v <= 1 && w <= 1)
                found = true;
            //else i = nabo som svarer til minste barysentriske koordinaten
            else
            {
                if(u<=v && u<=w && mTriangles[Ti].neighbours[0] != -1)
                    Ti = mTriangles[Ti].neighbours[0];
                if(v<=u && v<=w && mTriangles[Ti].neighbours[1] != -1)
                    Ti = mTriangles[Ti].neighbours[1];
                if(w<=u && w<=v && mTriangles[Ti].neighbours[2] != -1)
                    Ti = mTriangles[Ti].neighbours[2];
                else
                {
                    qDebug() << "Out of bounds";
                    found = true;
                }
            }
        } while (!found);
        float mP = u * mVertices[mTriangles[Ti].indexes[0]].getZ();
        float mQ = v * mVertices[mTriangles[Ti].indexes[1]].getZ();
        float mR = w * mVertices[mTriangles[Ti].indexes[2]].getZ();
        zReturn = 0.1 + (mP + mQ + mR);
        qDebug() << zReturn;
        //qDebug() << "Triangle found!" << mTriangles[i].indexes[0] << mTriangles[i].indexes[1] << mTriangles[i].indexes[2];
        return mTriangles[Ti];
    }
    else
    {
        qDebug() << "Out of bounds";
        zReturn = 0;
        return mTriangles[0];
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

void TriangleSurface::shaderToggle(GLuint ShaderId)
{
    mShaderId = ShaderId;
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

