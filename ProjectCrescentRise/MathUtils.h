#pragma once

#include <glm/glm.hpp>
#include <cmath>

class MathUtils
{
public:
    static constexpr float PI = 3.14159265359f;
    static constexpr float TWO_PI = 2.0f * PI;
    static constexpr float HALF_PI = PI * 0.5f;

    static glm::vec3 normalize(const glm::vec3& v)
    {
        float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        if (length > 0.0f) {
            return glm::vec3(v.x / length, v.y / length, v.z / length);
        }
        return v;
    }

    static glm::vec2 calculateUV(const glm::vec3& vertex)
    {
        float u = 0.5f + (std::atan2(vertex.z, vertex.x) / (2.0f * 3.14159265359f));
        float v = 0.5f - (std::asin(vertex.y) / 3.14159265359f);
        return glm::vec2(u, v);
    }

    static float lerp(float a, float b, float t)
    {
        return a + t * (b - a);
    }

    static float clamp(float value, float min, float max)
    {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    static float toRadians(float degrees)
    {
        return degrees * (PI / 180.0f);
    }

    static float toDegrees(float radians)
    {
        return radians * (180.0f / PI);
    }
};
