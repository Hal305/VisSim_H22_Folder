#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <vector>
#include "vertex.h"

enum CollisionBox{none, AABB};
class VisualObject : public QOpenGLFunctions_4_1_Core
{
public:
    VisualObject();
    VisualObject(GLuint ShaderId, GLuint TextureId);
    ~VisualObject();
    virtual void init(GLint matrixUniform)=0;
    virtual void draw()=0;
    void drawLines();
    //interaction movement
    virtual void move(float x, float y, float z){;};
    //simulation movement
    virtual void move(float dt){;};
    std::pair<float, float> getPosition2D();
    QVector3D getPosition3D();
    void setName (std::string name);
    std::string getName() const;
    QVector3D findNormalV(float x, float y, float z);
    virtual void setScale(float scale);
    QMatrix4x4 mMatrix;
    virtual GLuint getShaderId(){return mShaderId;}
    virtual GLuint getTexId(){return mTextureId;}

    CollisionBox mCollisionBox{none};
    QVector3D collisionBoxSize{};
    void getPlaneHeight(float z);

protected:
    std::vector<Vertex> mVertices;
    std::vector<GLuint> mIndices;

    GLuint mVAO{0};
    GLuint mVBO{0};
    GLuint mIBO{0};

    GLint mMatrixUniform{0};
    GLuint mShaderId;
    GLuint mTextureId;

    QMatrix4x4 mPosition;
    QMatrix4x4 mRotation;
    QMatrix4x4 mScale;
    //setup for simulation
    QVector3D mVelocity;

    std::string mName;
};

#endif // VISUALOBJECT_H
