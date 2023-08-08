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
// Provides the QGlScene class and the extendable qgl_callback namespace.
//
// AUTHORS:
// -------
//      Igor Nunes (https://github.com/thoga31)
//
// LICENSE:
// -------
//      GNU GPL V3.0
//------------------------------------------------------------------------------

#include "quickgl.hpp"
#include <iostream>
#include <cstring>

using namespace qgl;

namespace qgl::callback {
    QGlScene *scene;

    void bindInstance(QGlScene *scn) {
        scene = scn;
    }

    QGlScene* getInstance() {
        return scene;
    }

    void QGlDefaultCallback_Mouse(GLFWwindow* window, double xpos, double ypos) {
        if (getInstance()->getMouseData().first) {
            getInstance()->setMouseData(xpos, ypos, false);
        }

        float xoffset = xpos - getInstance()->getMouseData().lastX;
        float yoffset = getInstance()->getMouseData().lastY - ypos;     // reversed since y-coordinates go from bottom to top
        getInstance()->setMouseData(xpos, ypos, false);
        getInstance()->withCamera().processMouseMovement(xoffset, yoffset);
    }

    void QGlDefaultCallback_FramebufferSize(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void QGlDefaultCallback_Scroll(GLFWwindow* window, double xoffset, double yoffset) {
        getInstance()->withCamera().processMouseScroll(yoffset);
    }
}


const char* QGLException::what() const throw () { return message.c_str(); }


QGlAction QGlScene::QGlDefaultMethod_ProcessInput() {
    // Escape key to exit application
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(this->window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);

    // WASD keys to move around
    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS)
        this->camera.processKeyboard(FORWARD, this->deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS)
        this->camera.processKeyboard(BACKWARD, this->deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
        this->camera.processKeyboard(LEFT, this->deltaTime);
    if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
        this->camera.processKeyboard(RIGHT, this->deltaTime);

    return QGlAction::NO_ACTION;
}


QGlScene::QGlScene(const char* argv0) {
    this->path.call = fs::path(strncmp("./", argv0, 2) == 0 ? &argv0[2] : argv0);
    this->path.curr = fs::path(fs::current_path());
    this->path.full = this->path.curr / fs::path(this->path.call).remove_filename();
}


GLFWwindow* QGlScene::getWindow() {
    return this->window;
}


QGlMouseData& QGlScene::withMouseData() {
    return this->getMouseData();
}

QGlMouseData& QGlScene::getMouseData() {
    return this->mouse;
}


void QGlScene::setMouseData(float x, float y, bool first) {
    this->mouse = {x, y, first};
}


QGlShader& QGlScene::withProgram(string name) {
    if (!this->programs.contains(name)) {
        this->programs[name] = QGlShader(this->path.full.c_str());
    }
    return this->programs[name];
}


bool QGlScene::init_glfw() {
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

    this->attachFrameBufferSizeCallback();
    this->attachMouseButtonCallback();
    this->attachCursorPositionCallback();
    this->attachScrollCallback();

    // glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    return true;
}


#ifdef QGL_GLAD
int QGlScene::init_glad() {
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}
#endif


void QGlScene::initialize() {
    this->initialize(DEFAULT_SCR_HEIGHT, DEFAULT_SCR_WIDTH, DEFAULT_SCR_TITLE);
}


void QGlScene::initialize(const unsigned width, const unsigned height) {
    this->initialize(width, height, DEFAULT_SCR_TITLE);
}


void QGlScene::initialize(const unsigned width, const unsigned height, string title) {
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


bool QGlScene::launchSuccessful() {
    return this->success;
}


void QGlScene::run() {
    while (!glfwWindowShouldClose(this->getWindow())) {
        callback::bindInstance(this);
        this->preProcessInput(*this);
        this->processInput(*this);
        this->refresh(*this);
        glfwSwapBuffers(this->window);
        glfwPollEvents();
    }
}


void QGlScene::finalize() {
    glfwTerminate();
}


void QGlScene::setFrameBufferSizeCallback(GLFWframebuffersizefun callback, bool attachNow) {
    this->framebuffer_size_callback = callback;
    if (attachNow)
        this->attachFrameBufferSizeCallback();
}

void QGlScene::setMouseButtonCallback(GLFWmousebuttonfun callback, bool attachNow) {
    this->mousebtn_callback = callback;
    if (attachNow)
        this->attachMouseButtonCallback();
}

void QGlScene::setCursorPositionCallback(GLFWcursorposfun callback, bool attachNow) {
    this->mouse_callback = callback;
    if (attachNow)
        this->attachCursorPositionCallback();
}

void QGlScene::setScrollCallback(GLFWscrollfun callback, bool attachNow) {
    this->scroll_callback = callback;
    if (attachNow)
        this->attachScrollCallback();
}



void QGlScene::attachFrameBufferSizeCallback() {
    if (this->framebuffer_size_callback != nullptr)
        glfwSetFramebufferSizeCallback(this->window, this->framebuffer_size_callback);
}

void QGlScene::attachMouseButtonCallback() {
    if (this->mousebtn_callback != nullptr)
        glfwSetMouseButtonCallback(this->window, this->mousebtn_callback);
}

void QGlScene::attachCursorPositionCallback() {
    if (this->mouse_callback != nullptr)
        glfwSetCursorPosCallback(this->window, this->mouse_callback);
}

void QGlScene::attachScrollCallback() {
    if (this->scroll_callback != nullptr)
        glfwSetScrollCallback(this->window, this->scroll_callback);
}