#include <iostream>
#include "quickgl.hpp"

/* Example of how to extend the qgl_callback namespace.
 * getInstance() must be used to access the Scene instance currently bound. */
namespace qgl_callback {
    void foo(void) {
        GLFWwindow* w = getInstance()->getWindow();
    }
}

/* Defines what to do for each frame. */
void Scene::refresh(void) {
    // something
}

/* Test */
int main(int argc, char const *argv[]) {
    // Create instance
    Scene scene;

    // Initialize GLFW
    scene.initialize(1000, 500, "Teste");

    // Checks if the OpenGL scene was launched successfully
    if (!scene.launchSuccessful())
        std::cerr << "Launch was not successful." << std::endl;
    else
        scene.run();    // Runs the rendering cycle
    
    // The scene is automatically destroyed
    return 0;
}
