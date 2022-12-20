#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    void Update(glm::mat4 model, glm::vec3 translation, float rotation, glm::mat4 Sun, glm::mat4 Mercury, glm::mat4 Venus, glm::mat4 Earth, glm::mat4 Mars, glm::mat4 Jupiter, glm::mat4 Saturn, glm::mat4 Uranus, glm::mat4 Neptune, glm::mat4 Pluto);
    void setSpeed(glm::vec3 spd) { m_speed = spd; };
    void setRotation(float rot) { angle = rot; }
    void setAngles(float angX, float angY) { xpos = angX; ypos = angY; };
    void setView(glm::mat4 vw) { view = vw; };
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    float xpos, ypos;

    glm::vec3 speed = { 1.0, 0.0, 1.0 };
    glm::vec3 dist = { 1.0, 0.0, 1.0 };
    float angle = 0.0f;
    void setViewState(bool state) { viewState = state; };
    bool getColState() { return colState; };
    bool viewState = 0;
    bool colState = 0;
    bool changeState = 0;

    glm::vec3 Pscale;
    glm::quat Protation;
    glm::vec3 Pskew;
    glm::vec4 Pperspective;

    bool CollisionCheck(glm::vec3 translation, glm::vec3 Ptranslation, float rotation);

    glm::vec3 Stranslation;
    glm::vec3 Metranslation;
    glm::vec3 Vtranslation;
    glm::vec3 Etranslation;
    glm::vec3 Matranslation;
    glm::vec3 Jtranslation;
    glm::vec3 Satranslation;
    glm::vec3 Utranslation;
    glm::vec3 Ntranslation;
    glm::vec3 Plutranslation;

private:

    glm::vec3 m_speed = glm::vec3(0., 0., 0.);
    double x = 0.0;
    double y = 10.0;
    double z = -16.0;
    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
