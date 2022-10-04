#ifndef LIGHT_H
#define LIGHT_H

#include "visualobject.h"

///Simple Light class that has a small mesh and some light parameters
class Light : public VisualObject
{
public:
    ///Light class with mesh and light parameters
    Light();
    Light(GLuint ShaderId, GLuint TextureId);
    virtual void init(GLint MatrixUniform) override;
    virtual void draw() override;

    GLfloat mAmbientStrength{0.3f};
    QVector3D mAmbientColor{0.3f, 0.3f, 0.9f};  //Grey

    GLfloat mLightStrength{0.7f};
    QVector3D mLightColor{0.7f, 0.0f, 0.9f};

    GLfloat mSpecularStrength{0.6f};
    GLint mSpecularExponent{4};
protected:
    std::vector<GLuint> mIndices;
    GLuint mIBO{0};
};

#endif // LIGHT_H
