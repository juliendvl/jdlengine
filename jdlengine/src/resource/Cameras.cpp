#include "resource/Cameras.hpp"

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

void PerspectiveCamera::setFOV(float value)
{
    m_fov = glm::max(0.0f, value);
    updateProjectionMatrix();
}

void PerspectiveCamera::setAspectRatio(float value)
{
    m_aspectRatio = glm::max(0.0f, value);
    updateProjectionMatrix();
}

void PerspectiveCamera::setNearPlane(float value)
{
    m_nearPlane = glm::max(0.0f, value);
    updateProjectionMatrix();
}

void PerspectiveCamera::setFarPlane(float value)
{
    m_farPlane = glm::max(0.0f, value);
    updateProjectionMatrix();
}

void PerspectiveCamera::updateProjectionMatrix()
{
    m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
}


// --- OrbitCamera CLASS ---

OrbitCamera::OrbitCamera(const std::string& name) : PerspectiveCamera(name)
{
    updateViewMatrix();
}

void OrbitCamera::setCenter(const glm::vec3& center)
{
    m_center = center;
    updateViewMatrix();
}

void OrbitCamera::setRadius(float value)
{
    m_radius = glm::max(0.0f, value);
    updateViewMatrix();
}

void OrbitCamera::setAzimuth(float value)
{
    m_azimuth = value;
    updateViewMatrix();
}

void OrbitCamera::setPolar(float value)
{
    constexpr float HALF_PI = glm::half_pi<float>();

    m_polar = glm::clamp(value, -HALF_PI, HALF_PI);
    updateViewMatrix();
}

glm::vec3 OrbitCamera::getPosition() const
{
    glm::vec3 localPosition = m_radius * glm::vec3(
        glm::cos(m_polar) * glm::cos(m_azimuth),
        glm::sin(m_polar),
        glm::cos(m_polar) * glm::sin(m_azimuth)
    );

    return m_center + localPosition;
}

void OrbitCamera::updateViewMatrix()
{
    m_viewMatrix = glm::lookAt(getPosition(), m_center, glm::vec3(0.0f, 1.0f, 0.0f));
}

} // namespace resource
} // namespace jdl
