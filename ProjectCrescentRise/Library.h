#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#define PI 3.1415926535897932384626433f
inline static float degreesToRadians(float t_degrees)
{
    return t_degrees * (PI / 180.0f);
}
inline static float radiansToDegrees(float t_radians)
{
    return t_radians * 180.f / PI;
}

struct Transform {
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

	void moveAlongForward(glm::vec3 t_disp);
    void moveAlongForwardPlanet(glm::vec3 t_disp);
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
