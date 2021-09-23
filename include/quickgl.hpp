#ifndef QUICKGL_H
#define QUICKGL_H

#include <glad/glad.h>
#define GLM_FORCE_RADIANS

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "camera.hpp"

#ifdef QGL_FREETYPE
    #include <ft2build.h>
    #include FT_FREETYPE_H
#endif

class Scene {
private:
    bool success = false;
    GLFWwindow* window;
    unsigned scr_height;
    unsigned scr_width;

    /* Timing */
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float currentFrame;

    Camera camera;

    bool init_glfw(void);
    int init_glad(void);

protected:
    const unsigned DEFAULT_SCR_HEIGHT = 800;
    const unsigned DEFAULT_SCR_WIDTH  = 600;

    /* Callbacks */
    void setFrameBufferSizeCallback(GLFWframebuffersizefun);
    void setMouseButtonCallback(GLFWmousebuttonfun);
    void setCursorPositionCallback(GLFWcursorposfun);
    void setScrollCallback(GLFWscrollfun);

public:
    /* Initialization, render cycle and finalization */
    void initialize(void);
    void initialize(const unsigned, const unsigned);
    void finalize(void);
    bool launchSuccessful(void);
    void run(void);
    void refresh(void);

    /* Properties and fields */
    GLFWwindow* getWindow(void);

    /* Callbacks */
    virtual void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    virtual void mousebtn_callback(GLFWwindow *window, int button, int action, int mods);
    virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif