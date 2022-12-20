#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <stack>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "sphere.h"
#include "mesh.h"
#include "Light.h"

#define numVBOs 2;
#define numIBs 2;


class Graphics
{
  public:
    Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    void HierarchicalUpdate2(double dt);
    void Render();

    Camera* getCamera() { return m_camera; }
    Mesh* getShip() { return m_mesh; }

    float sAngle;

  private:
    std::string ErrorString(GLenum error);

    bool collectShPrLocs();
    void ComputeTransforms (double dt, std::vector<float> speed, std::vector<float> dist,
        std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, 
        glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat);

    stack<glm::mat4> modelStack;
    glm::mat4 m_ship;

    Camera *m_camera;
    Shader *m_shader;

    GLuint m_projectionMatrix;
    GLuint m_viewMatrix;
    GLuint m_modelMatrix;
    GLuint m_normalMatrix;
    GLuint m_positionAttrib;
    GLuint m_colorAttrib;
    GLuint m_tcAttrib;
    GLuint m_hasTexture;
    GLuint m_hasNormal;
   
    GLuint globalAmbientLocation;
    GLuint lightAmbientLocation;
    GLuint lightDiffuseLocation;
    GLuint lightSpecularLocation;
    GLuint lightPositionLocation;
    
    GLuint materialAmbientLocation;
    GLuint materialDiffuseLocation;
    GLuint materialSpecularLocation; 
    GLuint materialShininessLocation;

    Light* m_light;

    Sphere* m_skybox;

    Sphere* m_sun;

    Sphere* m_mercury;
    Sphere* m_venus;

    Sphere* m_earth;
    Sphere* m_luna;

    Sphere* m_mars;
    Sphere* m_deimos;
    Sphere* m_phobos;

    Sphere* m_jupiter;
    Sphere* m_ganymede;
    Sphere* m_callisto;
    Sphere* m_io;
    Sphere* m_europa;

    Sphere* m_ceres;

    Sphere* m_saturn;
    Sphere* m_ring;
    Sphere* m_titan;
    Sphere* m_Smoon;
    Sphere* m_Smoon2;

    Sphere* m_uranus;
    Sphere* m_Umoon;
    Sphere* m_Umoon2;

    Sphere* m_neptune;
    Sphere* m_Nmoon;
    Sphere* m_Nmoon2;

    Sphere* m_pluto;
    Sphere* m_charon;

    Sphere* m_haumea;
    Sphere* m_eris;

    Object* m_object;

    Mesh* m_mesh;

    glm::mat4 m_colS;
    glm::mat4 m_colMe;
    glm::mat4 m_colV;
    glm::mat4 m_colE;
    glm::mat4 m_colMa;
    glm::mat4 m_colJ;
    glm::mat4 m_colSa;
    glm::mat4 m_colU;
    glm::mat4 m_colN;
    glm::mat4 m_colPlu;
    glm::mat4 m_colCe;



    std::vector<float> speedS;
    std::vector<float> distS;
    std::vector<float> rotSpeedS;
    std::vector<float> scaleS;
    glm::mat4 tmatS;
    glm::mat4 rmatS;
    glm::mat4 smatS;
    glm::vec3 rotVectorS;

    std::vector<float> speedSB;
    std::vector<float> distSB;
    std::vector<float> rotSpeedSB;
    std::vector<float> scaleSB;
    glm::mat4 tmatSB;
    glm::mat4 rmatSB;
    glm::mat4 smatSB;
    glm::vec3 rotVectorSB;

    std::vector<float> speedE;
    std::vector<float> distE;
    std::vector<float> rotSpeedE;
    std::vector<float> scaleE;
    glm::mat4 tmatE;
    glm::mat4 rmatE;
    glm::mat4 smatE;
    glm::vec3 rotVectorE;

    std::vector<float> speedL;
    std::vector<float> distL;
    std::vector<float> rotSpeedL;
    std::vector<float> scaleL;
    glm::mat4 tmatL;
    glm::mat4 rmatL;
    glm::mat4 smatL;
    glm::vec3 rotVectorL;

    std::vector<float> speedM;
    std::vector<float> distM;
    std::vector<float> rotSpeedM;
    std::vector<float> scaleM;
    glm::mat4 tmatM;
    glm::mat4 rmatM;
    glm::mat4 smatM;
    glm::vec3 rotVectorM;

    std::vector<float> speedMe;
    std::vector<float> distMe;
    std::vector<float> rotSpeedMe;
    std::vector<float> scaleMe;
    glm::mat4 tmatMe;
    glm::mat4 rmatMe;
    glm::mat4 smatMe;
    glm::vec3 rotVectorMe;

    std::vector<float> speedV;
    std::vector<float> distV;
    std::vector<float> rotSpeedV;
    std::vector<float> scaleV;
    glm::mat4 tmatV;
    glm::mat4 rmatV;
    glm::mat4 smatV;
    glm::vec3 rotVectorV;

    std::vector<float> speedMa;
    std::vector<float> distMa;
    std::vector<float> rotSpeedMa;
    std::vector<float> scaleMa;
    glm::mat4 tmatMa;
    glm::mat4 rmatMa;
    glm::mat4 smatMa;
    glm::vec3 rotVectorMa;

    std::vector<float> speedDei;
    std::vector<float> distDei;
    std::vector<float> rotSpeedDei;
    std::vector<float> scaleDei;
    glm::mat4 tmatDei;
    glm::mat4 rmatDei;
    glm::mat4 smatDei;
    glm::vec3 rotVectorDei;

    std::vector<float> speedPhob;
    std::vector<float> distPhob;
    std::vector<float> rotSpeedPhob;
    std::vector<float> scalePhob;
    glm::mat4 tmatPhob;
    glm::mat4 rmatPhob;
    glm::mat4 smatPhob;
    glm::vec3 rotVectorPhob;

    std::vector<float> speedGany;
    std::vector<float> distGany;
    std::vector<float> rotSpeedGany;
    std::vector<float> scaleGany;
    glm::mat4 tmatGany;
    glm::mat4 rmatGany;
    glm::mat4 smatGany;
    glm::vec3 rotVectorGany;

    std::vector<float> speedEur;
    std::vector<float> distEur;
    std::vector<float> rotSpeedEur;
    std::vector<float> scaleEur;
    glm::mat4 tmatEur;
    glm::mat4 rmatEur;
    glm::mat4 smatEur;
    glm::vec3 rotVectorEur;

    std::vector<float> speedCall;
    std::vector<float> distCall;
    std::vector<float> rotSpeedCall;
    std::vector<float> scaleCall;
    glm::mat4 tmatCall;
    glm::mat4 rmatCall;
    glm::mat4 smatCall;
    glm::vec3 rotVectorCall;

    std::vector<float> speedIo;
    std::vector<float> distIo;
    std::vector<float> rotSpeedIo;
    std::vector<float> scaleIo;
    glm::mat4 tmatIo;
    glm::mat4 rmatIo;
    glm::mat4 smatIo;
    glm::vec3 rotVectorIo;

    std::vector<float> speedJ;
    std::vector<float> distJ;
    std::vector<float> rotSpeedJ;
    std::vector<float> scaleJ;
    glm::mat4 tmatJ;
    glm::mat4 rmatJ;
    glm::mat4 smatJ;
    glm::vec3 rotVectorJ;

    std::vector<float> speedSa;
    std::vector<float> distSa;
    std::vector<float> rotSpeedSa;
    std::vector<float> scaleSa;
    glm::mat4 tmatSa;
    glm::mat4 rmatSa;
    glm::mat4 smatSa;
    glm::vec3 rotVectorSa;

    std::vector<float> scaleRing;
    glm::mat4 smatRing;

    std::vector<float> speedTitan;
    std::vector<float> distTitan;
    std::vector<float> rotSpeedTitan;
    std::vector<float> scaleTitan;
    glm::mat4 tmatTitan;
    glm::mat4 rmatTitan;
    glm::mat4 smatTitan;
    glm::vec3 rotVectorTitan;

    std::vector<float> speedU;
    std::vector<float> distU;
    std::vector<float> rotSpeedU;
    std::vector<float> scaleU;
    glm::mat4 tmatU;
    glm::mat4 rmatU;
    glm::mat4 smatU;
    glm::vec3 rotVectorU;

    std::vector<float> speedN;
    std::vector<float> distN;
    std::vector<float> rotSpeedN;
    std::vector<float> scaleN;
    glm::mat4 tmatN;
    glm::mat4 rmatN;
    glm::mat4 smatN;
    glm::vec3 rotVectorN;

    std::vector<float> speedPlu;
    std::vector<float> distPlu;
    std::vector<float> rotSpeedPlu;
    std::vector<float> scalePlu;
    glm::mat4 tmatPlu;
    glm::mat4 rmatPlu;
    glm::mat4 smatPlu;
    glm::vec3 rotVectorPlu;

    std::vector<float> speedC;
    std::vector<float> distC;
    std::vector<float> rotSpeedC;
    std::vector<float> scaleC;
    glm::mat4 tmatC;
    glm::mat4 rmatC;
    glm::mat4 smatC;
    glm::vec3 rotVectorC;

    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 skew;
    glm::vec3 translation;
    glm::vec4 perspective;

};

#endif /* GRAPHICS_H */
