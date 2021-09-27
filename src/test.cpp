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

#include <iostream>
#include "quickgl.hpp"

using namespace qgl;

/* Example of how to extend the qgl_callback namespace.
 * getInstance() must be used to access the Scene instance currently bound. */
namespace qgl::callback {
    void foo(GLFWwindow *window, int button, int action, int mods) {
        getInstance()->finalize();
    }
}

/* Defines what to do for each frame. */
void Scene::refresh(void) {
    // some graphical code...

    /* It is NOT necessary to swap buffers nor poll events.
     * This is automatically done by quickGL. */
}

/* Test */
int main(int argc, char const *argv[]) {
    // Create instance
    Scene scene;

    // Initialize GLFW
    scene.initialize(1000, 500, "Teste");

    // Define a função de callback dos botões do rato
    scene.setMouseButtonCallback(qgl::callback::foo);

    // Checks if the OpenGL scene was launched successfully
    if (!scene.launchSuccessful())
        std::cerr << "Launch was not successful." << std::endl;
    else
        scene.run();    // Runs the rendering cycle
    
    // The scene is automatically destroyed, hence it's NOT necessary to call glfwTerminate()
    return 0;
}
