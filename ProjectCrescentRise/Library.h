#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform {
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
};

inline glm::mat4 ToModelMatrix(const Transform& t) {
    glm::mat4 M(1.0f);
    M = glm::translate(M, t.position);
    // For 2D you might only need rotation.z:
    M = glm::rotate(M, glm::radians(t.rotation.z), { 0,0,1 });
    // If you want full 3D order:
    M = glm::rotate(M, glm::radians(t.rotation.y), { 0,1,0 });
    M = glm::rotate(M, glm::radians(t.rotation.x), { 1,0,0 });
    M = glm::scale(M, t.scale);
    return M;
}
