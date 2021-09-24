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

using namespace std;

class Scene {
private:
    bool success = false;
    GLFWwindow* window;
    
    unsigned scr_height;
    unsigned scr_width;
    string   scr_title;

    /* Timing */
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float currentFrame;

    Camera camera;

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

    /* Callbacks */

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

    /* Callbacks */
    void setFrameBufferSizeCallback(GLFWframebuffersizefun);
    void setMouseButtonCallback(GLFWmousebuttonfun);
    void setCursorPositionCallback(GLFWcursorposfun);
    void setScrollCallback(GLFWscrollfun);
};


/* === NAMESPACE qgl_callback ===
 * Namespace to be extended with callback functions that might need to refer to
 * the Scene instance.
 * At each iteration, the Scene will bind itself to this namespace.
 * */
namespace qgl_callback {
    Scene* scene;

    void bindInstance(Scene *scn) {
        scene = scn;
    }

    Scene* getInstance(void) {
        return scene;
    }
}

#endif