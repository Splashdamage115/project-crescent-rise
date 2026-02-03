#include "Library.h"
#include "PlanetSurface.h"

void Transform::moveAlongForwardPlanet(glm::vec3 t_disp)
{
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    if (PlanetSurface::s_instance && PlanetSurface::s_instance->getTransform())
    {
        up = glm::normalize(position - PlanetSurface::s_instance->getTransform()->position);
    }

    float yaw = degreesToRadians(rotation.y);

    glm::vec3 arbitrary = glm::vec3(0.0f, 0.0f, 1.0f);
    if (fabs(glm::dot(up, arbitrary)) > 0.999f) arbitrary = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 rightBase = glm::normalize(glm::cross(up, arbitrary));
    glm::vec3 forwardBase = glm::normalize(glm::cross(rightBase, up));

    glm::quat qYaw = glm::angleAxis(yaw, up);
    glm::vec3 forwardYaw = qYaw * forwardBase;

    glm::vec3 rightYaw = glm::normalize(glm::cross(forwardYaw, up));

    position += t_disp.x * rightYaw;
    position += t_disp.y * up;
    position += t_disp.z * forwardYaw;
}

void Transform::moveAlongForward(glm::vec3 t_disp)
{
    glm::quat forward = glm::quat(glm::vec3(0.0f, degreesToRadians(rotation.y), 0.0f));
    position += t_disp * forward;
}
