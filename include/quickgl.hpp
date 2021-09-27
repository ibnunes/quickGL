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

#ifndef QUICKGL_H
#define QUICKGL_H

#ifdef QGL_GLAD
    #include <glad/glad.h>
    #define GLM_FORCE_RADIANS
#else
    #ifdef QGL_GLAD_LOCAL
        #include "glad/glad.h"
        #define QGL_GLAD
        #define GLM_FORCE_RADIANS
    #endif
#endif

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

#include "camera.hpp"

#ifdef QGL_FREETYPE
    #include <ft2build.h>
    #include FT_FREETYPE_H
#endif

namespace qgl {
using namespace std;


class QGLException : public exception {
public:
    QGLException(const string &msg) : message(msg) {}
    virtual const char* what() const throw ();
private:
    const string message;
};


typedef struct {
    float lastX;
    float lastY;
    bool  first;
} mouse_data;

/* === CLASS Scene ===
 * A new instance creates an OpenGL window with independent properties.
 * */
class Scene {
private:
    bool success = false;       // Flag: indicates if launch was successful

    GLFWwindow* window;         // The OpenGL window reference
    
    unsigned scr_height;        // Window height
    unsigned scr_width;         // Window width
    string   scr_title;         // Window title

    /* Timing */
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float currentFrame;

    mouse_data mouse;           // Mouse last absolute position data

    Camera camera;              // Camera manager

    GLFWframebuffersizefun framebuffer_size_callback = nullptr;
    GLFWmousebuttonfun     mousebtn_callback         = nullptr;
    GLFWcursorposfun       mouse_callback            = nullptr;
    GLFWscrollfun          scroll_callback           = nullptr;

    bool init_glfw(void);
    #ifdef QGL_GLAD
    int init_glad(void);
    #endif

protected:
    const unsigned DEFAULT_SCR_HEIGHT = 800;
    const unsigned DEFAULT_SCR_WIDTH  = 600;
    const string   DEFAULT_SCR_TITLE  = "";

public:
    /* Initialization, render cycle and finalization */
    void initialize(void);
    void initialize(const unsigned, const unsigned);
    void initialize(const unsigned, const unsigned, string);
    void finalize(void);
    bool launchSuccessful(void);
    void run(void);
    virtual void refresh(void);

    /* Properties and fields */
    GLFWwindow* getWindow(void);
    mouse_data getMouseData(void);

    /* Callbacks */
    void setFrameBufferSizeCallback(GLFWframebuffersizefun);
    void setMouseButtonCallback(GLFWmousebuttonfun);
    void setCursorPositionCallback(GLFWcursorposfun);
    void setScrollCallback(GLFWscrollfun);

    // Constructor and destructor
    Scene() {};
    ~Scene() { this->finalize(); };
};


/* === NAMESPACE qgl::callback ===
 * Namespace to be extended with callback functions that might need to refer to
 * the Scene instance.
 * At each iteration, the Scene will bind itself to this namespace.
 * */
namespace callback {
    void bindInstance(Scene*);
    Scene* getInstance(void);
}

}

#endif