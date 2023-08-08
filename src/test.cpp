//------------------------------------------------------------------------------
//
// quickGL - A quick and easy to use OpenGL wrapper
//
// TEST SUITE
//    test.cpp
//
// DESCRIPTION:
// -----------
// Tests the basic functionality of quickGL and demonstrates the qgl_callback
// namespace extension and access to getInstante() method.
//
// AUTHORS:
// -------
//      Igor Nunes (https://github.com/thoga31)
//
// LICENSE:
// -------
//      GNU GPL V3.0
//------------------------------------------------------------------------------

#define QGL_GLAD_LOCAL
#include "quickgl.hpp"

#include <iostream>

using namespace qgl;

/* Example of how to extend the qgl_callback namespace.
 * getInstance() must be used to access the QGlScene instance currently bound. */
namespace qgl::callback {
    void foo(GLFWwindow *window, int button, int action, int mods) {
        getInstance()->finalize();
    }
}


/* Defines what to do with the inputs fetched by OpenGL at each frame. */
void myPreInput(QGlScene& cls) {
    // Some code to execute before processing the input...
}


/* Defines what to do with the inputs fetched by OpenGL at each frame. */
QGlAction myInput(QGlScene& cls) {
    /* You can use the default method within your own */
    QGlAction result = cls.QGlDefaultMethod_ProcessInput();

    // Some code to process the input...

    return result;
}


/* Defines what to do for each frame. */
void myRefresh(QGlScene& cls) {
    // some graphical code...

    /* It is NOT necessary to swap buffers nor poll events.
     * This is automatically done by quickGL. */
}



int main(int argc, char const *argv[]) {
    // Create instance
    QGlScene scene(argv[0]);
    scene.preProcessInput = myPreInput;
    scene.processInput    = myInput;
    scene.refresh         = myRefresh;

    // Initialize GLFW
    scene.initialize(1000, 500, "Teste");

    // Defines new mouse button click callback function
    scene.setMouseButtonCallback(qgl::callback::foo);

    // Checks if the OpenGL scene was launched successfully
    if (!scene.launchSuccessful())
        std::cerr << "Launch was not successful." << std::endl;
    else
        scene.run();    // Runs the rendering cycle

    // The scene is automatically destroyed, hence it's NOT necessary to call glfwTerminate()
    return 0;
}
