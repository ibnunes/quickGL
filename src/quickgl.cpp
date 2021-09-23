#include "quickgl.hpp"



GLFWwindow* Scene::getWindow(void) {
    return this->window;
}

bool Scene::init_glfw(int width, int height, const char* title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    this->window = glfwCreateWindow(width, height, title, NULL, NULL);
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

int Scene::init_glad(void) {
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}



void Scene::initialize(void) {
    this->initialize(DEFAULT_SCR_HEIGHT, DEFAULT_SCR_WIDTH);
}

void Scene::initialize(unsigned height, unsigned width) {
    this->scr_height = height;
    this->scr_width  = width;

    this->success = true;
}

bool Scene::launchSuccessful(void) {
    return this->success;
}

void Scene::run(void) {
    while (!glfwWindowShouldClose(this->getWindow())) {
        this->refresh();
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