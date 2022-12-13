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

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_positionAttrib;
    GLint m_colorAttrib;
    GLint m_tcAttrib;
    GLint m_hasTexture;

    Sphere* m_skybox;

    Sphere* m_sun;
    Sphere* m_earth;
    Sphere* m_luna;
    Sphere* m_mercury;
    Sphere* m_venus;
    Sphere* m_mars;

    Sphere* m_jupiter;
    Sphere* m_ganymede;
    Sphere* m_callisto;
    Sphere* m_io;
    Sphere* m_europa;

    Sphere* m_ceres;

    Sphere* m_saturn;
    Sphere* m_titan;

    Sphere* m_uranus;
    Sphere* m_neptune;
    Sphere* m_pluto;

    Object* m_object;

    Mesh* m_mesh;


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

    std::vector<float> speedP;
    std::vector<float> distP;
    std::vector<float> rotSpeedP;
    std::vector<float> scaleP;
    glm::mat4 tmatP;
    glm::mat4 rmatP;
    glm::mat4 smatP;
    glm::vec3 rotVectorP;

    std::vector<float> speedP1;
    std::vector<float> distP1;
    std::vector<float> rotSpeedP1;
    std::vector<float> scaleP1;
    glm::mat4 tmatP1;
    glm::mat4 rmatP1;
    glm::mat4 smatP1;
    glm::vec3 rotVectorP1;

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
