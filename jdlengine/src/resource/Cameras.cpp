#include "resource/Cameras.hpp"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>


namespace jdl
{
namespace resource
{

// --- PerspectiveCamera CLASS ---

PerspectiveCamera::PerspectiveCamera(const std::string& name) : Camera(name)
{
    updateProjectionMatrix();
}

void PerspectiveCamera::setFOV(float angle)
{
    m_fov = glm::max(0.0f, angle);
    updateProjectionMatrix();
}

void PerspectiveCamera::setAspectRatio(float ratio)
{
    m_aspectRatio = glm::max(0.0f, ratio);
    updateProjectionMatrix();
}

void PerspectiveCamera::setNearPlane(float near)
{
    m_nearPlane = glm::max(0.0f, near);
    updateProjectionMatrix();
}

void PerspectiveCamera::setFarPlane(float far)
{
    m_farPlane = glm::max(0.0f, far);
    updateProjectionMatrix();
}

void PerspectiveCamera::updateProjectionMatrix()
{
    m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
    m_projectionMatrix[1][1] *= -1;
}


// --- OrbitCamera CLASS ---
static const float MAX_PITCH = glm::radians(89.0f);

OrbitCamera::OrbitCamera(const std::string& name) : PerspectiveCamera(name)
{
    updateViewMatrix();
}

glm::vec3 OrbitCamera::getPosition() const
{
    // Compute the position on the orbit sphere
    float camX = m_radius * cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
    float camY = m_radius * sin(glm::radians(m_pitch));
    float camZ = m_radius * cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));

    return m_center + glm::vec3(camX, camY, camZ);
}

void OrbitCamera::setCenter(const glm::vec3& center)
{
    m_center = center;
    updateViewMatrix();
}

void OrbitCamera::setRadius(float radius)
{
    m_radius = glm::max(0.0f, radius);
    updateViewMatrix();
}

void OrbitCamera::setYaw(float yaw)
{
    m_yaw = yaw;
    updateViewMatrix();
}

void OrbitCamera::setPitch(float pitch)
{
    m_pitch = glm::clamp(pitch, -MAX_PITCH, MAX_PITCH);
    updateViewMatrix();
}

void OrbitCamera::updateRotation(float deltaYaw, float deltaPitch)
{
    m_yaw -= 0.5f * deltaYaw;
    m_pitch = glm::clamp(m_pitch - 0.5f * deltaPitch, -MAX_PITCH, MAX_PITCH);

    updateViewMatrix();
}

void OrbitCamera::updateViewMatrix()
{
    glm::vec3 position = getPosition();
    m_viewMatrix = glm::lookAt(position, m_center, glm::vec3(0.0f, 1.0f, 0.0f));
}

} // namespace resource
} // namespace jdl
