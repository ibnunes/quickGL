//------------------------------------------------------------------------------
//
// quickGL - A quick and easy to use OpenGL wrapper
//
// RUNTIME LIBRARIES PACKAGE
//    camera.cpp
//
// DESCRIPTION:
// -----------
// Main library.
// Provides the Scene class and the extendable qgl_callback namespace.
// 
// AUTHORS:
// -------
//      Igor Nunes (https://github.com/thoga31)
// 
// LICENSE:
// -------
//      GNU GPL V3.0
//------------------------------------------------------------------------------

#include <iostream>
#include "quickgl.hpp"

using namespace qgl;

namespace qgl::callback {
    Scene *scene;

    void bindInstance(Scene *scn) {
        scene = scn;
    }

    Scene* getInstance(void) {
        return scene;
    }
}

const char* QGLException::what(void) const throw () { return message.c_str(); }

GLFWwindow* Scene::getWindow(void) {
    return this->window;
}

mouse_data Scene::getMouseData(void) {
    return this->mouse;
}

bool Scene::init_glfw(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    this->window = glfwCreateWindow(this->scr_width, this->scr_height, this->scr_title.c_str(), NULL, NULL);
    if (this->window == NULL)
        return false;
    
    glfwMakeContextCurrent(this->window);
    
    if (this->framebuffer_size_callback != nullptr)
        this->setFrameBufferSizeCallback(this->framebuffer_size_callback);
    if (this->mousebtn_callback != nullptr)
        this->setMouseButtonCallback(this->mousebtn_callback);
    if (this->mouse_callback != nullptr)
        this->setCursorPositionCallback(this->mouse_callback);
    if (this->scroll_callback != nullptr)
        this->setScrollCallback(this->scroll_callback);

    // glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    return true;
}

#ifdef QGL_GLAD
int Scene::init_glad(void) {
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}
#endif


void Scene::initialize(void) {
    this->initialize(DEFAULT_SCR_HEIGHT, DEFAULT_SCR_WIDTH, DEFAULT_SCR_TITLE);
}

void Scene::initialize(const unsigned width, const unsigned height) {
    this->initialize(width, height, DEFAULT_SCR_TITLE);
}

void Scene::initialize(const unsigned width, const unsigned height, string title) {
    try {
        this->scr_height = height;
        this->scr_width  = width;
        this->scr_title  = title;

        if (!this->init_glfw())
            throw QGLException("Failed to create window.");

        #ifdef QGL_GLAD
        if (!this->init_glad())
            throw QGLException("Failed to initialize GLAD.");
        #endif

        this->success = true;
    } catch (QGLException &e) {
        std::cerr << "QuickGL Error: " << e.what() << std::endl;
        std::cerr << "QuickGL is aborting launch." << std::endl;
        this->success = false;
    }
}

bool Scene::launchSuccessful(void) {
    return this->success;
}

void Scene::run(void) {
    while (!glfwWindowShouldClose(this->getWindow())) {
        callback::bindInstance(this);
        this->refresh();
        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }
}

void Scene::finalize(void) {
    glfwTerminate();
}


void Scene::setFrameBufferSizeCallback(GLFWframebuffersizefun callback) {
    glfwSetFramebufferSizeCallback(this->window, callback);
}

void Scene::setMouseButtonCallback(GLFWmousebuttonfun callback) {
    glfwSetMouseButtonCallback(this->window, callback);
}

void Scene::setCursorPositionCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(this->window, callback);
}

void Scene::setScrollCallback(GLFWscrollfun callback) {
    glfwSetScrollCallback(this->window, callback);
}