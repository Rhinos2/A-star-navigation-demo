//
//
//

#include "../framework/GLMath.h"
#include "../framework/Program.h"

enum Shaders{ RED_SH = 1, PERFRAG = 2, TEXT_SH = 3, TOON_SH =4, RED_SH_SIMP =5, PERFRAG_D };
extern MYTRANS trans;
extern MYPROGRAM* prg;

void Uniforms(Shaders sdr){
    GLint uniform;
    if(sdr ==  PERFRAG_D){
        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "MODELVIEWPROJECTIONMATRIX");
        glUniformMatrix4fv(uniform, 1, GL_FALSE,
                           glm::value_ptr(trans.GetPRJmat() * trans.GetMVmat()));

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "NORMALMATRIX");
        glUniformMatrix3fv(uniform, 1, GL_FALSE, glm::value_ptr(trans.GetNormalmat()));

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "MODELVIEWMATRIX");
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(trans.GetMVmat() * trans.GetMODmat()));

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "LIGHTPOSITION");
        glm::vec4 Lpos4 = trans.GetVmat() * glm::vec4(1.0f, 0.5f, 0.2f, 0.0f);
        glm::vec3 Lposit = glm::vec3(Lpos4);
        //    glUniform3fv(uniform, 1, (float *) &Lposit);

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "LIGHTINTENSITY");
        glm::vec3 lInts(0.8f, 0.3f, 0.4f);
        glUniform3fv(uniform, 1, (float *) &lInts);
        glUniform3fv(uniform, 1, (float *) &Lposit);

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "Material.Shininess");
        GLfloat ms(180.0f);
        glUniform1f(uniform, ms);

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "Material.Ka");
        glm::vec3 ka(0.7f, 0.7f, 0.8f);
        glUniform3fv(uniform, 1, (float *) &ka);

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "Material.Kd");
        glm::vec3 kd(0.8f, 0.8f, 0.5f);
        glUniform3fv(uniform, 1, (float *) &kd);

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "Material.Ks");
        glm::vec3 ks(0.7f, 0.7f, 0.7f);
        glUniform3fv(uniform, 1, (float *) &ks);

    }
    if(sdr == TEXT_SH) {
        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "PROJECTIONMATRIX");
        //to load the matrix into the uniform
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(trans.GetPRJmat()));

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "NORMALMATRIX");
        //to load the matrix into the uniform
        glUniformMatrix3fv(uniform, 1, GL_FALSE, glm::value_ptr(trans.GetNormalmat()));

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "MODELVIEWMATRIX");
        //to load the matrix into the uniform
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(trans.GetMVmat() * trans.GetMODmat()));

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "LIGHTPOSITION");
        glm::vec4 Lpos4 = trans.GetVmat() * glm::vec4(1.0f, 0.5f, 0.0f, 0.0f);
        glm::vec3 Lposit = glm::vec3(Lpos4);
        glUniform3fv(uniform, 1, (float *) &Lposit);

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "DIFFUSE");
        glUniform1i(uniform, 0);

    }
    else if(sdr == RED_SH_SIMP) {
        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "MODELVIEWPROJECTIONMATRIX");
        glUniformMatrix4fv(uniform, 1, GL_FALSE,
                           glm::value_ptr(trans.GetPRJmat() * trans.GetMVmat()));

    }
    else if(sdr == RED_SH){
        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "PROJECTIONMATRIX");
        //to load the matrix into the uniform
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(trans.GetPRJmat()));

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "NORMALMATRIX");
        //to load the matrix into the uniform
        glUniformMatrix3fv(uniform, 1, GL_FALSE, glm::value_ptr(trans.GetNormalmat()));

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "MODELVIEWMATRIX");
        //to load the matrix into the uniform
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(trans.GetMVmat() * trans.GetMODmat()));

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "LIGHTPOSITION");
        glm::vec4 Lpos4 = trans.GetVmat() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 Lposit = glm::vec3(Lpos4);
        glUniform3fv(uniform, 1, (float *) &Lposit);

    }
    else if(sdr == PERFRAG) {
///////////////////////////////////////////////////////////////////////////////////////PERFRAG SHD
        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "MODELVIEWPROJECTIONMATRIX");
        glUniformMatrix4fv(uniform, 1, GL_FALSE,
                           glm::value_ptr(trans.GetPRJmat() * trans.GetMVmat()));

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "NORMALMATRIX");
        glUniformMatrix3fv(uniform, 1, GL_FALSE, glm::value_ptr(trans.GetNormalmat()));

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "MODELVIEWMATRIX");
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(trans.GetMVmat() * trans.GetMODmat()));

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "LIGHTPOSITION");
        glm::vec4 Lpos4 = trans.GetVmat() * glm::vec4(2.0f, 5.0f, 2.0f, 0.0f);
        glm::vec3 Lposit = glm::vec3(Lpos4);
        //    glUniform3fv(uniform, 1, (float *) &Lposit);

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "LIGHTINTENSITY");
        glm::vec3 lInts(0.8f, 0.3f, 0.4f);
        glUniform3fv(uniform, 1, (float *) &lInts);
        glUniform3fv(uniform, 1, (float *) &Lposit);

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "Material.Shininess");
        GLfloat ms(180.0f);
        glUniform1f(uniform, ms);

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "Material.Ka");
        glm::vec3 ka(0.7f, 0.7f, 0.8f);
        glUniform3fv(uniform, 1, (float *) &ka);

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "Material.Kd");
        glm::vec3 kd(0.8f, 0.8f, 0.5f);
        glUniform3fv(uniform, 1, (float *) &kd);

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "Material.Ks");
        glm::vec3 ks(0.7f, 0.7f, 0.7f);
        glUniform3fv(uniform, 1, (float *) &ks);
    }
    /////////////////////////////////////////////////////////////////////////////////TOON
    else if(sdr == TOON_SH){
        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "MODELVIEWPROJECTIONMATRIX");
        glUniformMatrix4fv(uniform, 1, GL_FALSE,
                           glm::value_ptr(trans.GetPRJmat() * trans.GetMVmat()));

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "NORMALMATRIX");
        glUniformMatrix3fv(uniform, 1, GL_FALSE, glm::value_ptr(trans.GetNormalmat()));

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "MODELVIEWMATRIX");
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(trans.GetMVmat() * trans.GetMODmat()));

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "Light.position");
        glm::vec4 Lpos4 = trans.GetVmat() * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 Lposit = glm::vec3(Lpos4);
        glUniform3fv(uniform, 1, (float *) &Lposit);

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "Light.intensity");
        glm::vec3 lInts(0.6f, 0.5f, 0.5f);
        glUniform3fv(uniform, 1, (float *) &lInts);


        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "Ka");
        glm::vec3 ka(0.5f, 0.5f, 0.2f);
        glUniform3fv(uniform, 1, (float *) &ka);

        uniform = prg->PROGRAM_get_uniform_location((GLchar *) "Kd");
        glm::vec3 kd(0.3f, 0.3f, 0.3f);
        glUniform3fv(uniform, 1, (float *) &kd);

    }
}