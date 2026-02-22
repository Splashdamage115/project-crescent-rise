#pragma once
#include "Library.h"
#include "Window.h"

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction; // normalized

    // Build a ray from the center of the camera using the current view matrix.
    static Ray FromCamera()
    {
        glm::mat4 invView = glm::inverse(Window::Get().GetView());

        Ray r;
        r.origin    =  glm::vec3(invView[3]);       // camera world position
        r.direction = -glm::vec3(invView[2]);        // camera forward (-Z axis)
        r.direction =  glm::normalize(r.direction);
        return r;
    }

    // Returns true if the ray hits a sphere. Writes the hit distance into `t`.
    bool hitsphere(const glm::vec3& center, float radius, float& t) const
    {
        glm::vec3 oc = origin - center;
        float a  = glm::dot(direction, direction);
        float b  = 2.0f * glm::dot(oc, direction);
        float c  = glm::dot(oc, oc) - radius * radius;
        float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0.0f) return false;
        t = (-b - glm::sqrt(discriminant)) / (2.0f * a);
        return t >= 0.0f;
    }

    // Returns the world-space point along the ray at distance t.
    glm::vec3 at(float t) const { return origin + direction * t; }
};