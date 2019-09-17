#include "DemoApp.h"
#include <errno.h>
#include <vector>

#include "../framework/Program.h"
#include "../framework/GLMath.h"
#include "../framework/GLES.h"
#include "../framework/OBJ.h"
#include "../framework/Timing.h"
#include "../framework/GraphNode.h"
#include "../framework/NavGraph.h"
#include "../framework/AstarSearch.h"
#include "../framework/Linear.h"


#define VERTEX_SHADER_SIMP ( char * )"simply_red.vs"
#define FRAGMENT_SHADER_SIMP ( char * )"simply_red.fs"

#define VERTEX_SHADER_PER ( char * )"perfrag.vs"
#define FRAGMENT_SHADER_PER ( char * )"perfrag.fs"

#define VERTEX_SHADER_TOON ( char * )"toon.vs"
#define FRAGMENT_SHADER_TOON ( char * )"toon.fs"



enum Shaders{ RED_SH = 1, PERFRAG = 2, TEXT_SH = 3, TOON_SH =4, RED_SH_SIMP =5, PERFRAG_D };
extern void Uniforms(Shaders sdr);
#define DEBUG_SHADERS 1

glm::vec3 touch (0.0f, 0.0f, 0.0f);
glm::vec3 rot_angle  (0.0f, 0.0f, 0.0f);
GLint viewport_matrix[ 4 ];

Graph_SearchAStar* Astar = NULL;
Linear*    LinearInterp;
MYPROGRAM* prg = NULL;
GObjects GOs;

unsigned char double_tap = 0;

MYTRANS trans;

glm::vec3 eye(0.0f, 6.0f, -6.0f);
glm::vec3 cent(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::mat4 LOOKAT;

glm::vec3 EyePoint;
glm::mat4 ViewToWorldMatrix;
glm::mat4 WorldToViewMatrix;

glm::vec3 ClickPos(0.0f, 0.0f, 0.0f);
glm::vec3 OldPos(0.0f, -0.1f, 0.0f);
glm::vec3 NewPos(0.0f, 0.0f, 0.0f);
glm::vec3 vecTrn(0.0f, 0.0f, 0.0f);
float orient = 90.0f;
int source = 15;//initial character position
float deltaY = -0.1f;
const float aboveXZ = 0.4f;

glm::mat4 LookAtR( glm::vec3 eye, glm::vec3 lookAt, glm::vec3 up );
glm::mat3 SetColumns(glm::mat3 rotate, glm::vec3 col1, glm::vec3 col2, const glm::vec3 col3 );
glm::vec3 GetPickRay( float sx, float sy, float fov, float width, float height );
//////////////////////////////////////////////////////////////////////////////
void DemoAppInit(GLchar*  apkFilePath, GLint width, GLint height ) {

    GLchar buffer[256];

    strcpy(buffer, apkFilePath);
    atexit(DemoAppExit);

    GLES::GLES_Init();
    TimingData::init();
    /////////////
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glGetIntegerv(GL_VIEWPORT, viewport_matrix);

/////////////////////////////////////////////////////////////////////////////////// perspective prj
    trans.SetMmatrixMode(PROJECTION_MAT);
    trans.SetIdentity();
    trans.SetPerspective(60.0f, (GLfloat) viewport_matrix[2], (GLfloat) viewport_matrix[3], 0.1f, 100.0f);

///////////////////////////////////////////////////////////////////////////////// view (look at)

    trans.SetMmatrixMode(MODEL_VIEW_MAT);
    trans.SetIdentity();
    LOOKAT = LookAtR(eye, cent, up);

/////////////////////////////////////////////////////////////shader progs creation
    prg = new MYPROGRAM((GLchar *) "VertOnlyProg");
    if (0 != prg->PROGRAM_create(VERTEX_SHADER_SIMP, FRAGMENT_SHADER_SIMP, buffer, DEBUG_SHADERS))
       exit(1);
    if (0 != prg->PROGRAM_link(DEBUG_SHADERS))
        exit(2);
    GOs.AddProgram(std::string("VertOnlyProg"), prg);

    prg = new MYPROGRAM((GLchar *) "VertOnlyProgPer");
    if (0 != prg->PROGRAM_create(VERTEX_SHADER_PER, FRAGMENT_SHADER_PER, buffer, DEBUG_SHADERS))
        exit(1);
    if (0 != prg->PROGRAM_link(DEBUG_SHADERS))
        exit(2);
    GOs.AddProgram(std::string("VertOnlyProgPer"), prg);

    prg = new MYPROGRAM((GLchar *) "VertOnlyToon");
    if (0 != prg->PROGRAM_create(VERTEX_SHADER_TOON, FRAGMENT_SHADER_TOON, buffer, DEBUG_SHADERS))
        exit(1);
    if (0 != prg->PROGRAM_link(DEBUG_SHADERS))
        exit(2);
    GOs.AddProgram(std::string("VertOnlyToon"), prg);

//////////////////////////////////////////////////////////////////////// obj files loading
    if (!GOs.ReadOBJ(buffer, (GLchar *) "terrainNav3.obj"))
        exit(2);
   GLint last_mesh2 = GOs.LookupMeshNum(std::string( "terrainNav3.obj"));
    GOs.GObjs[last_mesh2]->UpdateBoundMesh();
    GOs.GObjs[last_mesh2]->BuildVBO();
    GOs.GObjs[last_mesh2]->BuildVBO(45.0f, 0.2f);//walkable slope angle and elevation
    GOs.GObjs[last_mesh2]->NavigationGraph();
    GOs.GObjs[last_mesh2]->BindLoadBuffers(false, true);//
    GOs.GObjs[last_mesh2]->SetMeshAttribs(prg);
    std::vector<TRI_NODE> trianglesNav =  GOs.GObjs[last_mesh2]->GetTriangles(true);
    vecTrn = trianglesNav[source].centGrav;

    if (!GOs.ReadOBJ(buffer, (GLchar *) "terrain3.obj"))
        exit(2);
    GLint   last_mesh1 = GOs.LookupMeshNum(std::string( "terrain3.obj"));
    GOs.GObjs[last_mesh1]->UpdateBoundMesh();
    GOs.GObjs[last_mesh1]->BuildVBO();
    GOs.GObjs[last_mesh1]->BindLoadBuffers(false, false);//
    GOs.GObjs[last_mesh1]->SetMeshAttribs(prg);

    if (!GOs.ReadOBJ(buffer, (GLchar *) "FinalMomo.obj"))
        exit(2);
    GLint last_mesh3 = GOs.LookupMeshNum(std::string( "FinalMomo.obj"));
    GOs.GObjs[last_mesh3]->UpdateBoundMesh();
    GOs.GObjs[last_mesh3]->BuildVBO();

    GOs.GObjs[last_mesh3]->BindLoadBuffers(false, false);
    GOs.GObjs[last_mesh3]->SetMeshAttribs(prg);

    if (!GOs.ReadOBJ(buffer, (GLchar *) "RhightMomo.obj"))
        exit(2);
    last_mesh3 = GOs.LookupMeshNum(std::string( "RhightMomo.obj"));
    GOs.GObjs[last_mesh3]->UpdateBoundMesh();
    GOs.GObjs[last_mesh3]->BuildVBO();

    GOs.GObjs[last_mesh3]->BindLoadBuffers(false, false);
    GOs.GObjs[last_mesh3]->SetMeshAttribs(prg);

    if (!GOs.ReadOBJ(buffer, (GLchar *) "LeftMomo.obj"))
        exit(2);
    last_mesh3 = GOs.LookupMeshNum(std::string( "LeftMomo.obj"));
    GOs.GObjs[last_mesh3]->UpdateBoundMesh();
    GOs.GObjs[last_mesh3]->BuildVBO();

    GOs.GObjs[last_mesh3]->BindLoadBuffers(false, false);
    GOs.GObjs[last_mesh3]->SetMeshAttribs(prg);

    if (!GOs.ReadOBJ(buffer, (GLchar *) "sphereV.obj"))
        exit(2);
    GLint last_mesh = GOs.LookupMeshNum(std::string( "sphereV.obj"));
    GOs.GObjs[last_mesh]->UpdateBoundMesh();
    GOs.GObjs[last_mesh]->BuildVBO();
    GOs.GObjs[last_mesh]->BindLoadBuffers(false, false);
    GOs.GObjs[last_mesh]->SetMeshAttribs(prg);

    if (!GOs.ReadOBJ(buffer, (GLchar *) "TextR.obj"))
        exit(2);
    GLint last_mesh4 = GOs.LookupMeshNum(std::string( "TextR.obj"));
    GOs.GObjs[last_mesh4]->UpdateBoundMesh();
    GOs.GObjs[last_mesh4]->BuildVBO();
    GOs.GObjs[last_mesh4]->BindLoadBuffers(false, false);
    GOs.GObjs[last_mesh4]->SetMeshAttribs(prg);

/////////////////////////////////////////////////////////////////////////////Navigation
    SparseGraph graph(true);
    GLint mesh_inxNav = GOs.LookupMeshNum(std::string("terrainNav3.obj"));
    std::vector<TRI_NODE> triNav = GOs.GObjs[mesh_inxNav]->GetTriangles(true);
    for( int n = 0; n < triNav.size(); n++){
        Vector2D pos(triNav[n].centGrav.x, triNav[n].centGrav.z);
        NavGraphNode NewNode(n, pos);
        graph.AddNode(NewNode);
        std::vector<TRI_NODE> triNav2 = GOs.GObjs[mesh_inxNav]->GetTriangles(true);
        for( int k = 0; k < triNav2.size(); k++){
            if(n == k) continue; // no edges to itself
            float zSeparation = triNav[n].centGrav.z - triNav2[k].centGrav.z;
            float xSeparation = triNav[n].centGrav.x - triNav2[k].centGrav.x;
            float dist = sqrt(zSeparation*zSeparation + xSeparation * xSeparation);
            if(dist > 3.3f ) continue;//create edges with close neighbors only
            int from = triNav[n].NodeID;
            float cost;
            int to = triNav2[k].NodeID;
            if(triNav2[k].Reachability == 1)
                cost = 3 * dist;
            else if(triNav2[k].Reachability == 2)
                cost = 2 * dist;
            else
                cost = dist;
            NavGraphEdge NewEdge(from, to, cost);
            graph.AddEdge(NewEdge);
        }

    }
    Astar = new Graph_SearchAStar(graph);
    LinearInterp = new Linear();
}

void DemoAppDraw( void ) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    TimingData::get()->update();

    trans.SetMmatrixMode(MODEL_VIEW_MAT);
    trans.SetIdentity();
    trans.PushMatrix();
    trans.SetLookAtR(LOOKAT);
    trans.Rotate(rot_angle.x, glm::vec3(1.0f, 0.0f, 0.0f));
    trans.Rotate(rot_angle.z, glm::vec3(0.0f, 0.0f, 1.0f));
    trans.Scale(glm::vec3(0.5f));


    GLint mesh_ind2 = GOs.LookupMeshNum(std::string("terrain3.obj"));
    prg = GOs.GetProgram(std::string("VertOnlyProgPer"));
    glUseProgram(prg->GetPID());
    GOs.GObjs[mesh_ind2]->BindBuffers();
    Uniforms( PERFRAG_D);
    GOs.GObjs[mesh_ind2]->DrawMesh();
    trans.PopMatrix();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
///////////////////////////////////////////////////////////
    trans.SetMmatrixMode(MODEL_VIEW_MAT);
    trans.SetIdentity();
    trans.PushMatrix();
    trans.SetLookAtR(LOOKAT);
    trans.Rotate(rot_angle.x, glm::vec3(1.0f, 0.0f, 0.0f));
    trans.Rotate(rot_angle.z, glm::vec3(0.0f, 0.0f, 1.0f));
    trans.Scale(glm::vec3(0.5f));
    trans.Translate(glm::vec3(0.0f, 0.02f, 0.0f));
    GLint mesh_ind = GOs.LookupMeshNum(std::string("terrainNav3.obj"));
    prg = GOs.GetProgram(std::string("VertOnlyProgPer"));
    glUseProgram(prg->GetPID());
    GOs.GObjs[mesh_ind]->BindBuffers();
    Uniforms(PERFRAG);
    GOs.GObjs[mesh_ind]->DrawNavArea();
    trans.PopMatrix();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    trans.SetMmatrixMode(MODEL_VIEW_MAT);
    trans.SetIdentity();
    trans.PushMatrix();
    trans.SetLookAtR(LOOKAT);
    trans.Rotate(rot_angle.x, glm::vec3(1.0f, 0.0f, 0.0f));
    trans.Rotate(rot_angle.z, glm::vec3(0.0f, 0.0f, 1.0f));
    trans.Scale(glm::vec3(0.5f));
    ////////////////////////////////////////////////////////////////////////////////////////Evaluate
    bool bAnim = LinearInterp->Evaluate(TimingData::get()->averageFrameDuration,  vecTrn, orient);
    trans.Translate(vecTrn);
    trans.Rotate(orient, glm::vec3(0.0f, 1.0f, 0.0f));

    GLint mesh_ind1;
    static int count = -1;
    if(bAnim){
        count++;
        if(count == 0 || count == 1 || count == 2 || count == 3)
            mesh_ind1 = GOs.LookupMeshNum(std::string("RhightMomo.obj"));
        else if(count == 4 || count == 5 || count == 6 || count == 7)
            mesh_ind1 = GOs.LookupMeshNum(std::string("FinalMomo.obj"));
        else if(count == 8 || count == 9 || count == 10 || count == 11){
            mesh_ind1 = GOs.LookupMeshNum(std::string("LeftMomo.obj"));
            if(count == 11)  count = -1;
        }
    }
    else {
        mesh_ind1 = GOs.LookupMeshNum(std::string("FinalMomo.obj"));
        count = -1;
    }

    prg = GOs.GetProgram(std::string("VertOnlyToon"));
    glUseProgram(prg->GetPID());

    GOs.GObjs[mesh_ind1]->BindBuffers();
    Uniforms(TOON_SH);

    GOs.GObjs[mesh_ind1]->DrawMesh();
    trans.PopMatrix();///Rh

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //////////////////////////////////////////////////////////
    if( double_tap ) {
        glm::vec3 PickRay = GetPickRay(touch.x, touch.y, 60.0f, viewport_matrix[2], viewport_matrix[3] );
        float tR = -EyePoint.z/PickRay.z;

        ClickPos = EyePoint + tR*PickRay;//orig

///////////////////////////////////////////////////////////////////////////////
        GLint mesh_indNav = GOs.LookupMeshNum(std::string("terrainNav3.obj"));
        if(ClickPos.y > 0)
            ClickPos.y +=  ClickPos.y/2.0f;
        ClickPos.x  = 2*  ClickPos.x;
        if(glm::abs(ClickPos.x) > 6.0f || ClickPos.y >6.0f || ClickPos.y < -7.0f){
            double_tap = 0;
            return;
        }

        NewPos.z = ClickPos.y;
        NewPos.y = -0.2f;
        NewPos.x = ClickPos.x;

////////////////////////////////////////////////////////////////

        std::vector<TRI_NODE> trianglesNav = GOs.GObjs[mesh_indNav]->GetTriangles(true);
        GLfloat bestDiffX;
        GLfloat bestDiffZ;
        GLfloat nearestZ ;
        int target = 0;
        bestDiffX = bestDiffZ = nearestZ = GOs.GObjs[mesh_indNav]->GetDimantion().x/2.0f;
        std::vector<TRI_NODE> bestMatches;
        glm::vec3 bestNewPos(0.0f, 0.0f, 0.0f);
        float temp;
        for(GLuint tri = 0; tri < trianglesNav.size(); tri++) {

            if(NewPos.z <= 0){
                if(trianglesNav[tri].centGrav.z > 0.0f) continue;
                temp = NewPos.z - trianglesNav[tri].centGrav.z;
                temp = fabsf(temp);
            }
            else{//Z>0
                if(trianglesNav[tri].centGrav.z <= 0.0f) continue;
                temp = NewPos.z - trianglesNav[tri].centGrav.z;
                temp = fabsf(temp);
            }
            if(bestDiffZ > temp) {
                bestDiffZ = temp;
                nearestZ = trianglesNav[tri].centGrav.z;
            }
        }

        float temp1;

        for(GLuint tri = 0; tri < trianglesNav.size(); tri++) {

            if ((fabsf(nearestZ - trianglesNav[tri].centGrav.z) < 0.01f ) || (fabsf(trianglesNav[tri].centGrav.z - nearestZ) < 0.01f))
                bestMatches.push_back(trianglesNav[tri]);
        }


        for(GLuint i = 0; i < bestMatches.size(); i++) {

            if (NewPos.x >= 0)
            {//X>0
                if (bestMatches[i].centGrav.x < 0.0f) continue;
                temp1 = NewPos.x - bestMatches[i].centGrav.x;
                temp1 = fabsf(temp1);
            } else
            {//X<0
                if (bestMatches[i].centGrav.x >= 0.0f) continue;
                temp1 = NewPos.x - bestMatches[i].centGrav.x;
                temp1 = fabsf(temp1);
            }
            if (bestDiffX > temp1) {
                bestDiffX = temp1;
                bestNewPos = bestMatches[i].centGrav;
                target = bestMatches[i].NodeID;//

            }

        }

//////////////////////////////////////////////////////////////////
        Astar->Reload();
        Astar->Search(source, target);
        Astar->PrintPthNodes();
        source = target;
        std::list<int> pathTo = Astar->GetPathToTarget();
        std::list<int>::iterator pos;
        std::vector<glm::vec3> NavPoints;
        for(pos = pathTo.begin(); pos != pathTo.end(); ++pos) {
            NavGraphNode node = Astar->GetGraph().GetNode(*pos);
            Vector2D vec = node.Pos();
            glm::vec3 v3(vec.x, aboveXZ, vec.y);
            NavPoints.push_back(v3);
        }
        std::vector<float> times;
        float delta = TimingData::get()->averageFrameDuration * 50;//update time intervals
        for(unsigned int tCount = 0; tCount < NavPoints.size(); tCount++)
            times.push_back(tCount * delta);

        LinearInterp->Clean();
        LinearInterp->Initialize(NavPoints, times, NavPoints.size());

        double_tap = 0;
        deltaY = -0.1f;

        OldPos.x = bestNewPos.x;
        OldPos.y = -0.1f;
        OldPos.z = bestNewPos.z;

        bestMatches.clear();
    }
    /////////////////////////////////////////////////////////////////////////////////////////////
    trans.SetMmatrixMode(MODEL_VIEW_MAT);
    trans.SetIdentity();
    trans.PushMatrix();
    trans.SetLookAtR(LOOKAT);
    trans.Rotate(rot_angle.x, glm::vec3(1.0f, 0.0f, 0.0f));
    trans.Rotate(rot_angle.z, glm::vec3(0.0f, 0.0f, 1.0f));
    trans.Scale(glm::vec3(0.5f));
    float zSeparation = vecTrn.z - OldPos.z;
    float xSeparation = vecTrn.x - OldPos.x;

    if((zSeparation*zSeparation + xSeparation * xSeparation) < 1.5f) {

        if(deltaY < 1.8f)
            deltaY += 0.05f;
        else
            deltaY = 1.8f;
        OldPos.y = deltaY; //up the ball goes
    }
    trans.Translate(OldPos);
    GLint mesh_indf = GOs.LookupMeshNum(std::string("sphereV.obj"));

    GOs.GObjs[mesh_indf]->BindBuffers();
    prg = GOs.GetProgram(std::string("VertOnlyProg"));

    glUseProgram(prg->GetPID());
    Uniforms(RED_SH_SIMP);

    GOs.GObjs[mesh_indf]->DrawMesh();
    trans.PopMatrix();///Rh

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    trans.SetMmatrixMode(MODEL_VIEW_MAT);
    trans.SetIdentity();
    trans.PushMatrix();
    trans.SetLookAtR(LOOKAT);

    trans.Rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f ));
    trans.Translate(glm::vec3(0.0f, 0.0f, -8.0f ));
    mesh_ind = GOs.LookupMeshNum(std::string("TextR.obj"));

    GOs.GObjs[mesh_ind]->BindBuffers();
    prg = GOs.GetProgram(std::string("VertOnlyProg"));

    glUseProgram(prg->GetPID());
    Uniforms(RED_SH_SIMP);

    GOs.GObjs[mesh_ind]->DrawMesh();
    trans.PopMatrix();///Rh

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
void DemoAppTouchBegan( float x, float y, int tap_count ) {
    if( tap_count == 2 )
        double_tap = 1;
    touch.x = x;
    touch.y = y;

}

void DemoAppTouchMoved( float x, float y, int tap_count ) {

    rot_angle.z += -( touch.x - x )/20.0f;
    rot_angle.x += -( touch.y - y )/20.0f;

    touch.x = x;
    touch.y = y;
}
void DemoAppTouchEnded( float x, float y, int tap_count ){

    double_tap = 0;
    touch.x = 0.0f;
    touch.y = 0.0f;
}

void DemoAppExit( void )
{
    if(prg != NULL)
        delete prg;
    if(Astar != NULL)
        delete Astar;
    if(LinearInterp != NULL)
        delete LinearInterp;
}

glm::mat4 LookAtR( glm::vec3 eye, glm::vec3 lookAt, glm::vec3 up )
{
    EyePoint = eye;
    // compute view vectors
    glm::vec3 view = lookAt - eye;
    glm::vec3  right;
    glm::vec3  viewUp;
    view = glm::normalize(view);

    right =  glm::cross(view, up );
    right = glm::normalize(right);
    viewUp =  glm::cross(right, view );
    viewUp = glm::normalize(viewUp);

   glm::mat3 rotate;
    rotate = SetColumns(rotate,  right, viewUp, -view );

    ViewToWorldMatrix = glm::mat4(rotate);

    ViewToWorldMatrix[0][3] =  ViewToWorldMatrix[1][3] = ViewToWorldMatrix[2][3] = 0.0;
    ViewToWorldMatrix[3][3] = 1.0;
    ViewToWorldMatrix[3][0] =  ViewToWorldMatrix[3][1] = ViewToWorldMatrix[3][2] = 0.0;

    // set translation (eye position)
    ViewToWorldMatrix[3][0] = eye.x;
    ViewToWorldMatrix[3][1] = eye.y;
    ViewToWorldMatrix[3][2] = eye.z;

    // world->view transform
    // set rotation
    rotate = glm::transpose(rotate);
    WorldToViewMatrix = glm::mat4(rotate);

    WorldToViewMatrix[0][3] =  WorldToViewMatrix[1][3] = WorldToViewMatrix[2][3] = 0.0;
    WorldToViewMatrix[3][3] = 1.0;
    WorldToViewMatrix[3][0] =  WorldToViewMatrix[3][1] = WorldToViewMatrix[3][2] = 0.0;

    glm::vec3 invEye = -(rotate*eye);
    WorldToViewMatrix[3][0] = invEye.x;//3 0
    WorldToViewMatrix[3][1] = invEye.y;//3 1
    WorldToViewMatrix[3][2] = invEye.z;//3 2
/////////////////////////////////////////////////////////////
    return WorldToViewMatrix;

}
glm::vec3 GetPickRay( float sx, float sy, float fov, float width, float height )
{
    float d = 1.0f/tanf(fov* PI/360.0f);
    float aspect = width/height;
    glm::vec3 viewPoint( 2.0f*aspect*sx/width - aspect, -2.0f*sy/height + 1.0f, -d);


    glm::mat4 RES = glm::affineInverse ( WorldToViewMatrix);

    glm::vec3 result;
    result.x = RES[0][0]*viewPoint.x + RES[1][0]*viewPoint.y + RES[2][0]*viewPoint.z + RES[3][0];
    result.y = RES[0][1]*viewPoint.x + RES[1][1]*viewPoint.y + RES[2][1]*viewPoint.z + RES[3][1];
    result.z = RES[0][2]*viewPoint.x + RES[1][2]*viewPoint.y + RES[2][2]*viewPoint.z + RES[3][2];

    viewPoint = result;
    return viewPoint-EyePoint;//orig //

}
glm::mat3 SetColumns(glm::mat3 rotate, glm::vec3 col1, glm::vec3 col2, const glm::vec3 col3 )
{
    rotate[0][0] = col1.x;
    rotate[0][1] = col1.y;
    rotate[0][2] = col1.z;

    rotate[1][0] = col2.x;
    rotate[1][1] = col2.y;
    rotate[1][2] = col2.z;

    rotate[2][0] = col3.x;
    rotate[2][1] = col3.y;
    rotate[2][2] = col3.z;

    return rotate;

}