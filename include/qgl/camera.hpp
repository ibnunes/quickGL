//------------------------------------------------------------------------------
//
// quickGL - A quick and easy to use OpenGL wrapper
//
// RUNTIME LIBRARIES PACKAGE
//    camera.hpp
//
// DESCRIPTION:
// -----------
// Manages the camera by giving easy methods to manipulate it.
//
// FROM:
// -----
//      Learn OpenGL (https://learnopengl.com/)
//
// DISCLAIMER: Changes to the original source have been made.
//------------------------------------------------------------------------------

#ifndef CAMERA_H
#define CAMERA_H

#include "qgl/common.hpp"
#include <vector>

/* Defines several possible options for camera movement.
 * Used as abstraction to stay away from window-system specific input methods. */
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};


/* Default camera values */
const float YAW         = -90.0f;
const float PITCH       =   0.0f;
const float SPEED       =   2.5f;
const float SENSITIVITY =   0.1f;
const float ZOOM        =  45.0f;


/* An abstract camera class that processes input and calculates the corresponding
 * Euler Angles, Vectors and Matrices for use in OpenGL. */
class QGlCamera {
private:
    // QGlCamera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Euler Angles
    float yaw;
    float pitch;

    // QGlCamera options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    // Calculates the front vector from the QGlCamera's (updated) Euler Angles
    void updateCameraVectors();

public:
    // Constructor with vectors
    QGlCamera(glm::vec3 = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 = glm::vec3(0.0f, 1.0f, 0.0f), float = YAW, float = PITCH);

    // Constructor with scalar values
    QGlCamera(float, float, float, float, float, float, float, float);

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 getViewMatrix();

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboard(Camera_Movement, float);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(float, float, GLboolean = true);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(float);

    QGlCamera& withPosition(glm::vec3);
    QGlCamera& withFront(glm::vec3);
    QGlCamera& withUp(glm::vec3);
    QGlCamera& withRight(glm::vec3);
    QGlCamera& withWorldUp(glm::vec3);
    QGlCamera& withYaw(float);
    QGlCamera& withPitch(float);
    QGlCamera& withMovementSpeed(float);
    QGlCamera& withMouseSensitivity(float);
    QGlCamera& withZoom(float);

    glm::vec3 getPosition();
    glm::vec3 getFront();
    glm::vec3 getUp();
    glm::vec3 getRight();
    glm::vec3 getWorldUp();
    float     getYaw();
    float     getPitch();
    float     getMovementSpeed();
    float     getMouseSensitivity();
    float     getZoom();
};

#endif