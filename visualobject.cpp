#include "visualobject.h"
#include "barycentriccalc.h"

VisualObject::VisualObject() {  }

VisualObject::~VisualObject() {
   glDeleteVertexArrays( 1, &mVAO );
   glDeleteBuffers( 1, &mVBO );
}

void VisualObject::drawLines()
{
    if(mIndices.empty())
    {
        glBindVertexArray(mVAO);
        glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
        glDrawArrays(GL_LINE_STRIP, 0, mVertices.size());
    }
    else
    {
        initializeOpenGLFunctions();
        glBindVertexArray( mVAO );
        // GL_FALSE for QMatrix4x4
        glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
        glDrawElements(GL_LINES, GLsizei(mIndices.size()), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));
    }
}

std::pair<float, float> VisualObject::getPosition2D()
{
    //For column based matrixes
    QVector4D col = mPosition.column(3);
    return std::pair<float, float> (col.x(), col.y());
}

QVector3D VisualObject::getPosition3D()
{
    QVector4D col = mPosition.column(3);
    QVector3D pos = {col.x(), col.y(), col.z()};
    return pos;
}

void VisualObject::setName(std::string name)
{
    mName = name;
}

std::string VisualObject::getName() const
{
    return mName;
}

void VisualObject::setScale(float scale)
{
    mScale.scale(QVector3D(scale,scale,scale));
}
