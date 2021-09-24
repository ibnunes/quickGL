#include "quickgl.hpp"

GLFWwindow* Scene::getWindow(void) {
    return this->window;
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
    
    this->setFrameBufferSizeCallback(this->framebuffer_size_callback);
    this->setMouseButtonCallback(this->mousebtn_callback);
    this->setCursorPositionCallback(this->mouse_callback);
    this->setScrollCallback(this->scroll_callback);

    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

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
    this->success = true;

    this->scr_height = height;
    this->scr_width  = width;
    this->scr_title  = title;

    if (!this->init_glfw()) {
        this->success = false;
    }

    #ifdef QGL_GLAD
    if (!this->init_glad()) {
        this->success = false;
    }
    #endif
}

bool Scene::launchSuccessful(void) {
    return this->success;
}

void Scene::run(void) {
    while (!glfwWindowShouldClose(this->getWindow())) {
        qgl_callback::bindInstance(this);
        this->refresh();
    }
}

void Scene::finalize(void) {
    glfwTerminate();
}


/*
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
*/