#pragma once
#include "Library.h"
#include "Window.h"

class GameObject;

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction; // normalized
    GameObject* gameObject = nullptr;
    glm::vec3 hitPoint = glm::vec3(0.0f);
    float distance = 0.0f;

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

    static Ray FromTransform(const glm::vec3& position, const glm::vec3& forward)
    {
        Ray r;
        r.origin = position;
        r.direction = glm::normalize(forward);
        return r;
	}

    static Ray CastTowardsPosition(const glm::vec3& origin, const glm::vec3& target)
    {
        Ray r;
        r.origin = origin;
        r.direction = glm::normalize(target - origin);
        return r;
	}

    static Ray castTowardsCamera(const glm::vec3& origin)
    {
        glm::mat4 invView = glm::inverse(Window::Get().GetView());
        Ray r;
        r.origin = origin;
        r.direction = -glm::vec3(invView[2]);
        r.direction = glm::normalize(r.direction);
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

    float distanceToPoint(const glm::vec3& other)
    {
        return 0.0f;
    }

    // Returns the world-space point along the ray at distance t.
    glm::vec3 at(float t) const { return origin + direction * t; }
};