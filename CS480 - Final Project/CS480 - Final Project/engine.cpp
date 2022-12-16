

#include "engine.h"
#include "glm/ext.hpp"

Engine::Engine(const char* name, int width, int height)
{
    m_WINDOW_NAME = name;
    m_WINDOW_WIDTH = width;
    m_WINDOW_HEIGHT = height;

}


Engine::~Engine()
{
    delete m_window;
    delete m_graphics;
    m_window = NULL;
    m_graphics = NULL;
}

bool Engine::Initialize()
{
    // Start a window
    m_window = new Window(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT);
    if (!m_window->Initialize())
    {
        printf("The window failed to initialize.\n");
        return false;
    }

    // Start the graphics
    m_graphics = new Graphics();
    if (!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT))
    {
        printf("The graphics failed to initialize.\n");
        return false;
    }

    glfwSetCursorPosCallback(m_window->getWindow(), cursorPositionCallBack);


    // No errors
    return true;
}

void Engine::Run()
{
    m_running = true;

    while (!glfwWindowShouldClose(m_window->getWindow()))
    {
        ProcessInput();
        Display(m_window->getWindow(), glfwGetTime());
        glfwPollEvents();
    }
    m_running = false;

}

void Engine::ProcessInput()
{
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window->getWindow(), true);

   // glfwSetScrollCallback(m_window, scroll_callback);

    //get mouse position
    //glfwGetCursorPos(m_window->getWindow(), &xpos, &ypos);
    //adjust the camera position according to arrow keys and mouse position
    //m_graphics->getCamera()->setView(glm::lookAt(glm::vec3(xcamPos, ycamPos + 20, zcamPos-40), glm::vec3((-xpos + 800) / 8, (-ypos + 400) / 8, 0.0), glm::vec3(0.0, 1.0, 0.0)));


    // Update camera animation here.


    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_RELEASE &&
        glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_RELEASE &&
        glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_RELEASE &&
        glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_RELEASE &&
        glfwGetKey(m_window->getWindow(), GLFW_KEY_Z) == GLFW_RELEASE &&
        glfwGetKey(m_window->getWindow(), GLFW_KEY_SPACE) == GLFW_RELEASE &&
        glfwGetKey(m_window->getWindow(), GLFW_KEY_Q) == GLFW_RELEASE &&
        glfwGetKey(m_window->getWindow(), GLFW_KEY_E) == GLFW_RELEASE)

        m_graphics->getCamera()->setSpeed(glm::vec3(0., 0., 0.));


    m_graphics->getShip()->setSpeed(glm::vec3(0., 0., 0.));
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        m_graphics->getShip()->setSpeed(glm::vec3(0., -10, 0.));
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
        m_graphics->getShip()->setSpeed(glm::vec3(0., 10, 0.));
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_Q) == GLFW_PRESS)
        m_graphics->getShip()->setSpeed(glm::vec3(10, 0., 0.));
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_E) == GLFW_PRESS)
        m_graphics->getShip()->setSpeed(glm::vec3(-10, 0., 0.));
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        m_graphics->getShip()->setSpeed(glm::vec3(0, 0, -10));
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        m_graphics->getShip()->setSpeed(glm::vec3(0, 0, 10));
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_graphics->getShip()->setSpeed(glm::vec3(0, 0, 60));
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        m_graphics->getShip()->setRotation(.04f);
    else
        m_graphics->getShip()->setRotation(0.0f);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        m_graphics->getShip()->setRotation(-.04f);
    if (glfwGetKey(m_window->getWindow(), GLFW_KEY_V) == GLFW_PRESS)
        m_graphics->getCamera()->setViewState(0);




}

void Engine::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}


void Engine::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

    glfwGetCursorPos(window, &xpos, &ypos);
    std::cout << "Position: (" << xpos << ":" << ypos << ")";
}

unsigned int Engine::getDT()
{
    //long long TimeNowMillis = GetCurrentTimeMillis();
    //assert(TimeNowMillis >= m_currentTimeMillis);
    //unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
    //m_currentTimeMillis = TimeNowMillis;
    //return DeltaTimeMillis;
    return glfwGetTime();
}

long long Engine::GetCurrentTimeMillis()
{
    //timeval t;
    //gettimeofday(&t, NULL);
    //long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
    //return ret;
    return (long long)glfwGetTime();
}

void Engine::Display(GLFWwindow* window, double time) {

    m_graphics->Render();
    m_window->Swap();
    m_graphics->HierarchicalUpdate2(time);
}

static void cursorPositionCallBack(GLFWwindow* window, double xpos, double ypos) {
    //cout << xpos << " " << ypos << endl;
}


