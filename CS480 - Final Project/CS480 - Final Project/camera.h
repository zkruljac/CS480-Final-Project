#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    void Update(unsigned int dt);
    void setSpeed(glm::vec3 spd) { m_speed = spd; };
    void setAngles(float angX, float angY) { xpos = angX; ypos = angY; };
    void setView(glm::mat4 vw) { view = vw; };
    glm::mat4 GetProjection();
    glm::mat4 GetView();
    float xpos, ypos;


private:

    glm::vec3 m_speed = glm::vec3(0., 0., 0.);
    double x = 0.0;
    double y = 10.0;
    double z = -16.0;
    glm::mat4 projection;
    glm::mat4 view;
};

#endif /* CAMERA_H */
