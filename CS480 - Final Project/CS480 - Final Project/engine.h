#ifndef ENGINE_H
#define ENGINE_H


#include <assert.h>
#include "window.h"
#include "graphics.h"

static void cursorPositionCallBack(GLFWwindow*, double xpos, double ypos);

class Engine
{
public:
    Engine(const char* name, int width, int height);

    ~Engine();
    bool Initialize();
    void Run();
    void ProcessInput();
    unsigned int getDT();
    long long GetCurrentTimeMillis();
    void Display(GLFWwindow*, double);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    double xpos, ypos;
    double xcamPos, ycamPos, zcamPos;

    void setSpeed(glm::vec3 spd) { speed = spd; };

private:
    // Window related variables
    Window* m_window;
    const char* m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;

    glm::vec3 speed = glm::vec3(0.f, 0.f, 0.f);


    Graphics* m_graphics;

    bool m_running;
};

#endif // ENGINE_H
