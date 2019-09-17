//
//
//

#ifndef _MYOBJ_H
#define _MYOBJ_H

#include <map>
#include <vector>
#include <string.h>

#include "../GLM/glm/glm.hpp"
#include "../GLM/glm/fwd.hpp"

#include "../GLM/glm/vec4.hpp"
#include "../GLM/glm/vec3.hpp"
#include "../GLM/glm/vec2.hpp"
#include "../GLM/glm/gtx/normal.hpp"
#include "../GLM/glm/gtx/string_cast.hpp"
#include "../GLM/glm/gtx/projection.hpp"
#include "../GLM/glm/gtx/norm.hpp"
#include "../GLM/glm/gtx/perpendicular.hpp"
#include "Texture.h"
#include "Program.h"
#include "Timing.h"
#include "GraphEdge.h"

struct OBJ_MATERIAL
{
    GLchar					m_name[64];				// newmtl
    glm::vec4			    ambient;						// Ka
    glm::vec4				diffuse;						// Kd
    glm::vec4				specular;						// Ks
    glm::vec3				transmission_filter;			// Tf

    GLint					illumination_model;				// illum
    GLfloat					dissolve;						// d
    GLfloat					specular_exponent;				// Ns
    GLfloat					optical_density;				// Ni

    GLchar					map_ambient_file[ 64 ];		// map_Ka
    GLchar					map_diffuse_file[ 64 ];		// map_Kd
    GLchar					map_specular_file[ 64 ];		// map_Ks

    bool bAmbient;
    bool bDiffuse;
    bool bSpecular;

    OBJ_MATERIAL() {bAmbient = bDiffuse = bSpecular = false;}
} ;

struct  TRI_NODE
{
    GLint NodeID;
    bool   bWalkable;
    int Reachability;
    GLuint PosInx[3];
    GLuint VtInx[3];
    GLuint VnInx[3];
    glm::vec3 triNormal;
    glm::vec3 centGrav;
    std::vector<NavGraphEdge> Edges;
    TRI_NODE(){
        PosInx[0] = PosInx[1] = PosInx[2] = 0;
        VtInx[0] = VtInx[1] = VtInx[2] = 0;
        NodeID = -1;
        bWalkable = false;
        centGrav.x = centGrav.y = centGrav.z = 0.0f;
        Edges.reserve(3);
        Reachability = 0;
    }
    inline TRI_NODE& operator --(int) {
        PosInx[0]--;PosInx[1]--; PosInx[2]--;
        VtInx[0]--;VtInx[1]--; VtInx[2]--;
        return *this;
    }
};
class GObjects;

class MESH
{
    friend class GObjects;
    GLchar                      o_name[64]; //o
    GLchar                      UseMtl[64]; //usemtl
    glm::vec3			        min;
    glm::vec3			        max;
    glm::vec3			        dimension;
    GLint                       mode;

    std::vector<glm::vec3>      vecVPos;
    GLuint                      n_vecVPos;
    std::vector<glm::vec3>      vecVNorm;
    GLuint                      n_vecVNorm;
    std::vector<glm::vec2>      vecVTexCoords;

    std::vector<GLushort>        vecInxUV;
    std::vector<GLushort>        vecInxV;
    std::vector<GLushort>        vecInxWalk;
    std::vector<GLushort>        vecInxOptz;
    GLuint                       n_vecInx;
    GLint                        array_buffer_size;

    std::vector<TRI_NODE>             Triangles;
    std::vector<TRI_NODE>            TriWalkable;

    MYTEXTURE*                  pTextures;
    std::map<GLuint,GLuint>     VPind_to_VUind;
    std::map<GLuint,GLuint>     VUinx_to_VTinx;

    GLuint                      stride;
    std::vector<GLfloat>        vecVBO;
    std::vector<GLfloat>        vecVBOnavig;
    GLuint                      vboIds[2];
    std::vector<GLint>          attribs;
    GLint                       attribPos;
    GLint                       attribNor;
    GLint                       attribTex;
public:
    glm::vec3                   location;
    GLfloat			            radius;
    GLuint                       n_vecInxOptim;
    GLboolean 			useVt;// = false;

public:
    MESH(GLchar* name){
        strcpy(o_name, name);
        n_vecVPos = 0;  n_vecInx = 0; n_vecVNorm = 0;
        attribPos = attribNor = attribTex = - 10;
        mode = GL_TRIANGLES;
        n_vecInxOptim = 0;
    }
    ~MESH() {
    }
    GLint Mode() { return mode; }

    void BindTexture(MYTEXTURE* pTexture);
    bool LoadTextures(GLchar*  apkFilePath, MYTEXTURE* pTexture, GLuint texUnit, GLuint flags, GLchar filter, GLfloat anisotropic_filter);
    void UpdateBoundMesh();
    void BuildVBO();
    void BindLoadBuffers(bool tex, bool nav);
    void BindBuffers();
    void BuildVBO(const GLfloat walkableSlopeAngle, const GLfloat walkableElevation);
    void SetMeshAttribs( MYPROGRAM *prg);
    void DrawMesh();
    void DrawNavArea();
    glm::vec3 GetDimantion(){ return dimension; }
    GLushort* GetV_InxBuffer();
    std::vector<GLushort> GetUV_InxBuffer();
    GLushort* GetOptimInxBuffer();
    std::map<GLuint,GLuint>& GetUV_toV_Map() { return  VUinx_to_VTinx; };
    std::vector<glm::vec3>& GetVPosBuffer();
    std::vector<glm::vec3>& GetVNormBuffer();
    std::vector<glm::vec2>& GetVTexBuffer();
    GLuint VPosBufferSize() { return n_vecVPos; }
    GLuint TexBufferSize() { return vecVTexCoords.size(); }
    GLuint NormBufferSize() { return  n_vecVNorm; }
    GLuint InxBufferSize();// { return  n_vecInx; }
    std::vector<TRI_NODE>& GetTriangles(bool walkable = false) { if(!walkable) return Triangles; else return TriWalkable;}
    void NavigationGraph();
};
class GObjects{
    std::map<std::string, GLint>    LookUpMesh;
    GLint                           numMesh;
public:
    TimingData*                     timingData;
    std::vector<MESH*>              GObjs;
    std::map<std::string, MYPROGRAM*>   mProgs;
    OBJ_MATERIAL                    Material;
    bool                            isMTL;
    GLchar                          MtlLib[64]; //mtllib

    GObjects() { isMTL = false; numMesh = 0;}
    ~GObjects();
    GLint LookupMeshNum(std::string mesh_name) { return LookUpMesh[mesh_name]; }
    void AddProgram(std::string name, MYPROGRAM* prg) { mProgs[name] = prg; }
    MYPROGRAM* GetProgram(std::string name) { return mProgs[name]; }
    bool ReadOBJ(GLchar*  apkFilePath, GLchar* obj_name);
    bool ReadMTL(GLchar*  apkFilePath, GLchar* mtl_name);
    GLchar* MtlFileName() { return &MtlLib[0]; }
};

#endif //_MYOBJ_H
