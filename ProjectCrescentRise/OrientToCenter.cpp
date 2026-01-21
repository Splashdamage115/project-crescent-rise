#include "OrientToCenter.h"
#include "PlanetSurface.h"
#include <glm/gtc/matrix_transform.hpp>

void OrientToCenter::Update()
{
    if (m_oriented || !transform || !PlanetSurface::s_instance)
        return;

    glm::vec3 planetCenter = PlanetSurface::s_instance->getTransform()->position;
    glm::vec3 currentPos = transform->position;
    
    // Ensure position has been set by instancer
    if (glm::length(currentPos) < 0.001f && glm::length(planetCenter) < 0.001f)
        return; 

    glm::vec3 up = glm::normalize(currentPos - planetCenter);

    // Align local Y with up
    glm::vec3 forward = (glm::abs(up.y) < 0.999f) ? glm::vec3(0, 0, 1) : glm::vec3(1, 0, 0);
    glm::vec3 right = glm::normalize(glm::cross(forward, up));
    forward = glm::cross(up, right);

    glm::mat3 rot(right, up, -forward);
    
    // Decompose into Euler ZYX (degrees)
    // Based on Transform's order: Rz * Ry * Rx
    float sy = -rot[0][2];
    float cy = sqrt(1.0f - sy * sy);
    
    float x, y, z;
    if (cy > 0.001f)
    {
        y = asin(sy);
        x = atan2(rot[1][2], rot[2][2]);
        z = atan2(rot[0][1], rot[0][0]);
    }
    else
    {
        y = asin(sy);
        x = 0;
        z = atan2(-rot[1][0], rot[1][1]);
    }

    transform->rotation = glm::degrees(glm::vec3(x, y, z));
    m_oriented = true;
}
