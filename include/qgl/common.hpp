#ifndef QGL_COMMON_H
#define QGL_COMMON_H

#if defined(QGL_GLAD)
    #include <glad/glad.h>
    #define GLM_FORCE_RADIANS
#elif defined(QGL_GLAD_LOCAL)
    #include "glad/glad.h"
    #define QGL_GLAD
    #define GLM_FORCE_RADIANS
#endif

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#endif