//------------------------------------------------------------------------------
//
// quickGL - A quick and easy to use OpenGL wrapper
//
// RUNTIME LIBRARIES PACKAGE
//    camera.cpp
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

#include "qgl/camera.hpp"

QGlCamera::QGlCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
    position(position),
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    worldUp(up),
    yaw(yaw),
    pitch(pitch),
    movementSpeed(SPEED),
    mouseSensitivity(SENSITIVITY),
    zoom(ZOOM)
{
    updateCameraVectors();
}


QGlCamera::QGlCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
    position(glm::vec3(posX, posY, posZ)),
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    worldUp(glm::vec3(upX, upY, upZ)),
    yaw(yaw),
    pitch(pitch),
    movementSpeed(SPEED),
    mouseSensitivity(SENSITIVITY),
    zoom(ZOOM)
{
    updateCameraVectors();
}


glm::mat4 QGlCamera::getViewMatrix() {
    return glm::lookAt(position, position + front, up);
}


void QGlCamera::processKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
}


void QGlCamera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // Update front, right and up Vectors using the updated Euler angles
    updateCameraVectors();
}


void QGlCamera::processMouseScroll(float yoffset) {
    if (zoom >= 1.0f && zoom <= 45.0f)
        zoom -= yoffset;
    if (zoom <= 1.0f)
        zoom = 1.0f;
    if (zoom >= 45.0f)
        zoom = 45.0f;
}


void QGlCamera::updateCameraVectors() {
    // Calculate the new front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front   = glm::normalize(front);

    // Also re-calculate the right and up vector
    right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up    = glm::normalize(glm::cross(right, front));
}


QGlCamera& QGlCamera::withPosition(glm::vec3 position) {
    this->position = position;
    return *this;
}

QGlCamera& QGlCamera::withFront(glm::vec3 front) {
    this->front = front;
    return *this;
}

QGlCamera& QGlCamera::withUp(glm::vec3 up) {
    this->up = up;
    return *this;
}

QGlCamera& QGlCamera::withRight(glm::vec3 right) {
    this->right = right;
    return *this;
}

QGlCamera& QGlCamera::withWorldUp(glm::vec3 worldUp) {
    this->worldUp = worldUp;
    return *this;
}

QGlCamera& QGlCamera::withYaw(float yaw) {
    this->yaw = yaw;
    return *this;
}

QGlCamera& QGlCamera::withPitch(float pitch) {
    this->pitch = pitch;
    return *this;
}

QGlCamera& QGlCamera::withMovementSpeed(float moveSpeed) {
    this->movementSpeed = moveSpeed;
    return *this;
}

QGlCamera& QGlCamera::withMouseSensitivity(float mouseSens) {
    this->mouseSensitivity = mouseSens;
    return *this;
}

QGlCamera& QGlCamera::withZoom(float zoom) {
    this->zoom = zoom;
    return *this;
}


glm::vec3 QGlCamera::getPosition()         { return this->position; }
glm::vec3 QGlCamera::getFront()            { return this->front; }
glm::vec3 QGlCamera::getUp()               { return this->up; }
glm::vec3 QGlCamera::getRight()            { return this->right; }
glm::vec3 QGlCamera::getWorldUp()          { return this->worldUp; }
float     QGlCamera::getYaw()              { return this->yaw; }
float     QGlCamera::getPitch()            { return this->pitch; }
float     QGlCamera::getMovementSpeed()    { return this->movementSpeed; }
float     QGlCamera::getMouseSensitivity() { return this->mouseSensitivity; }
float     QGlCamera::getZoom()             { return this->zoom; }
