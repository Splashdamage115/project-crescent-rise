#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "MathUtils.h"

struct Transform {
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

	void moveAlongForward(glm::vec3 t_disp)
	{
        glm::quat forward = glm::quat(glm::vec3(0.0f, MathUtils::toRadians(rotation.y), 0.0f));
        position += t_disp * forward;

	}
};

inline glm::mat4 ToModelMatrix(const Transform& t) {
    glm::mat4 M(1.0f);
    M = glm::translate(M, t.position);
    M = glm::rotate(M, glm::radians(t.rotation.z), { 0,0,1 });
    M = glm::rotate(M, glm::radians(t.rotation.y), { 0,1,0 });
    M = glm::rotate(M, glm::radians(t.rotation.x), { 1,0,0 });
    M = glm::scale(M, t.scale);
    return M;
}
