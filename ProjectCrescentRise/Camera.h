#pragma once

#include "Library.h"

class Camera
{
public:
    Camera() : m_position(0.0f, 0.0f, 5.0f), m_target(0.0f, 0.0f, 0.0f), m_up(0.0f, 1.0f, 0.0f)
    {}

    void setPosition(const glm::vec3& p) { m_position = p; }
    void setTarget(const glm::vec3& t) { m_target = t; }
    void setUp(const glm::vec3& u) { m_up = u; }

    glm::mat4 GetView() const { return glm::lookAt(m_position, m_target, m_up); }

    void setPerspective(float fovDeg, float aspect, float zNear, float zFar) {
        m_fov = fovDeg; m_aspect = aspect; m_near = zNear; m_far = zFar;
        m_proj = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    }
    void setAspect(float aspect) {
        m_aspect = aspect;
        m_proj = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
    }

    glm::mat4 GetProj() const { return m_proj; }

private:
    glm::vec3 m_position, m_target, m_up;
    float m_fov = 90.0f;
    float m_aspect = 16.0f / 9.0f;
    float m_near = 0.1f, m_far = 1000.0f; // clipping planes
    glm::mat4 m_proj = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
};

