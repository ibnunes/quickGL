//------------------------------------------------------------------------------
//
// quickGL - A quick and easy to use OpenGL wrapper
//
// RUNTIME LIBRARIES PACKAGE
//    camera.hpp
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

#ifndef QUICKGL_H
#define QUICKGL_H

#include "qgl/common.hpp"
#include "qgl/camera.hpp"
#include "qgl/shader.hpp"

#include <string>
#include <unordered_map>
#include <functional>
#include <filesystem>

namespace fs = std::filesystem;

namespace qgl {
using namespace std;


class QGLException : public exception {
public:
    QGLException(const string &msg) : message(msg) {}
    virtual const char* what() const throw ();
private:
    const string message;
};


struct QGlPathInfo {
    fs::path full;
    fs::path curr;
    fs::path call;
};


struct QGlMouseData {
    float lastX;
    float lastY;
    bool  first = true;
};


enum QGlAction {
    NO_ACTION,
    CAMERA_RESET,
    OPEN_FILE,
    CHANGE_COLOR
};


typedef unordered_map<string, QGlShader> QGlPrograms;


/* === NAMESPACE qgl::callback ===
 * Namespace to be extended with callback functions that might need to refer to
 * the QGlScene instance.
 * At each iteration, the QGlScene will bind itself to this namespace.
 * */
namespace callback {
    void QGlDefaultCallback_Mouse(GLFWwindow*, double, double);
    void QGlDefaultCallback_FramebufferSize(GLFWwindow*, int, int);
    void QGlDefaultCallback_Scroll(GLFWwindow*, double, double);
}


/*
 * A new instance creates an OpenGL window with independent properties.
 */
class QGlScene {
private:
    bool success = false;       // Flag: indicates if launch was successful

    QGlPathInfo path;

    GLFWwindow* window;         // The OpenGL window reference

    unsigned scr_height;        // Window height
    unsigned scr_width;         // Window width
    string   scr_title;         // Window title

    /* Timing */
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float currentFrame;

    QGlMouseData mouse;         // Mouse last absolute position data
    QGlCamera    camera;        // Camera manager

    QGlPrograms programs;       // Each program consists of a collection of shaders

    GLFWframebuffersizefun framebuffer_size_callback = qgl::callback::QGlDefaultCallback_FramebufferSize;
    GLFWmousebuttonfun     mousebtn_callback         = nullptr;
    GLFWcursorposfun       mouse_callback            = qgl::callback::QGlDefaultCallback_Mouse;
    GLFWscrollfun          scroll_callback           = qgl::callback::QGlDefaultCallback_Scroll;

    void attachFrameBufferSizeCallback();
    void attachMouseButtonCallback();
    void attachCursorPositionCallback();
    void attachScrollCallback();


    bool init_glfw();

#ifdef QGL_GLAD
    int init_glad();
#endif

protected:
    const unsigned DEFAULT_SCR_HEIGHT = 800;
    const unsigned DEFAULT_SCR_WIDTH  = 600;
    const string   DEFAULT_SCR_TITLE  = "";

public:
    /* Initialization, render cycle and finalization */
    void initialize();
    void initialize(const unsigned, const unsigned);
    void initialize(const unsigned, const unsigned, string);
    void finalize();

    bool launchSuccessful();

    void run();
    // virtual void refresh();
    // virtual QGlAction processInput();

    QGlAction QGlDefaultMethod_ProcessInput();
    function<void(QGlScene&)>      refresh         = [](QGlScene& scn){};   // Default: do nothing
    function<void(QGlScene&)>      preProcessInput = [](QGlScene& scn){};   // Default: do nothing
    function<QGlAction(QGlScene&)> processInput    = &QGlScene::QGlDefaultMethod_ProcessInput;

    /* Properties and fields */
    GLFWwindow*   getWindow();
    QGlMouseData& withMouseData();
    QGlMouseData& getMouseData();
    void          setMouseData(float, float, bool);

    QGlShader& withProgram(string);
    QGlCamera& withCamera() { return this->camera; }

    /* Callbacks */
    void setFrameBufferSizeCallback(GLFWframebuffersizefun, bool = true);
    void setMouseButtonCallback(GLFWmousebuttonfun, bool = true);
    void setCursorPositionCallback(GLFWcursorposfun, bool = true);
    void setScrollCallback(GLFWscrollfun, bool = true);

    // Constructor and destructor
    QGlScene() : QGlScene("") {};
    QGlScene(const char*);
    ~QGlScene() { this->finalize(); };
};


namespace callback {
    void bindInstance(QGlScene*);
    QGlScene* getInstance();
}

}

#endif