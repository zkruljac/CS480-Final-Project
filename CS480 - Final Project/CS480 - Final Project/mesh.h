#ifndef MESH_H
#define MESH_H

#include <vector>
#include "graphics_headers.h"
#include "Texture.h"

class Mesh
{
public:
    Mesh();
    Mesh(glm::vec3 pivot, const char* fname);
    Mesh(glm::vec3 pivot, const char* fname, const char* tname);

    ~Mesh();
    void Update(unsigned int dt);
    void Render(GLint posAttrib, GLint colAttrib);
    void Render(GLint positionAttribLoc, GLint colorAttribLoc, GLint tcAttribLoc, GLint hasTex);

    void setSpeed(glm::vec3 spd) { m_speed = spd; };
    void setTranslation(glm::vec3 pos) { model *= glm::translate(glm::mat4(1.0f), pos); }
    void setRotation(float rot) { angle = rot; };
    void setRoll(float rot) { roll = rot; };
    void setPitch(float rot) { pitch = rot; };
    float getAngle() { return angle; };

    glm::mat4 GetModel();

    bool InitBuffers();
    bool loadModelFromFile(const char* path);

    bool hasTex;
    GLuint getTextureID() { return m_texture->getTextureID(); }
    float angle = 0.0;
    float roll = 0.0;
    float pitch = 0.0;



private:
    glm::vec3 pivotLocation;
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    glm::vec3 m_speed = glm::vec3(0., 0., 0.);

    Texture* m_texture;

    GLuint vao;
};

#endif