//
//
//

#ifndef _GLMATH_H
#define _GLMATH_H

#include "GLES2/gl2.h"
#include <vector>
#include "../GLM/glm/fwd.hpp"
#include "../GLM/glm/glm.hpp"
#include "../GLM/glm/gtx/transform.hpp"
#include "../GLM/glm/gtc/matrix_transform.hpp"
#include "../GLM/glm/gtc/matrix_inverse.hpp"
#include "../GLM/glm/gtc/type_ptr.hpp"

#define PI         3.14159265359f
enum
{
    MODEL_VIEW_MAT = 0,
    PROJECTION_MAT = 1,
    MODEL_MAT = 2,
    NOT_SET = 3
};


struct GLMath{
    glm::mat4 Model;
    glm::mat4 Projection;
    glm::mat4 View;

    GLMath(){}

};
class MYTRANS{
    glm::mat4 model;
    glm::mat4 biasM;
    GLshort	mat_mode;
    GLshort	MV_pointer;
    GLshort	PRJ_pointer;
    GLshort	MOD_pointer;
    std::vector<glm::mat4>          MVm;
    std::vector<glm::mat4>			PRJm;
    std::vector<glm::mat4>			MODm;
    glm::mat4			Vm;
    glm::mat3			NORm;
    glm::mat4           InvMV;

public:
    MYTRANS();

    void SetMmatrixMode( GLshort mode ) { mat_mode = mode; }
    void SetPerspective(GLfloat fovy, GLfloat width, GLfloat height, GLfloat near, GLfloat far);
    void SetPerspective(glm::mat4 PrjMat);
    void SetOrthographic(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top,  GLfloat znear, GLfloat zfar);
    void SetOrthographic2D(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top);
    void SetLookAtOld(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
    void SetLookAtR(glm::mat4 lookAtR);
    void ChangeLookAt(glm::vec3 eye);
    const glm::mat4& GetModel() { return model; }
    glm::mat4& GetVmat() { return Vm; }
    glm::mat4& GetMVmat() { return MVm[MV_pointer]; }
    glm::mat4& GetPRJmat() {  return PRJm[PRJ_pointer]; }
    glm::mat4& GetMODmat() {  return MODm[MOD_pointer]; }
    glm::mat3& GetNormalmat();
    glm::mat4& GetInverseMV();
    void SetIdentity();
    void PushMatrix();
    void PopMatrix();
    void Rotate(GLfloat angle_rad, glm::vec3 axis );
    void Translate(glm::vec3 v);
    void Scale(glm::vec3 v);
};

#endif //_GLMATH_H

