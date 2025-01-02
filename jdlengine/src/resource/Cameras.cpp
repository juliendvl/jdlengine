#include "resource/Cameras.hpp"

#include <glm/gtc/matrix_transform.hpp>


namespace jdl
{
namespace resource
{

// --- PerspectiveCamera ---

PerspectiveCamera::PerspectiveCamera(const std::string& name)
    : Camera(name)
    , m_fov(glm::radians(45.0f))
    , m_aspectRatio(1.0f)
    , m_nearPlane(0.1f)
    , m_farPlane(1000.0f)
{
    updateProjectionMatrix();
}

void PerspectiveCamera::setFOV(float value)
{
    m_fov = glm::clamp(value, glm::radians(1.0f), glm::radians(89.0f));
    updateProjectionMatrix();
}

void PerspectiveCamera::setAspectRatio(float value)
{
    m_aspectRatio = glm::max(0.001f, value);
    updateProjectionMatrix();
}

void PerspectiveCamera::setNearPlane(float value)
{
    m_nearPlane = glm::max(1e-6f, value);
    updateProjectionMatrix();
}

void PerspectiveCamera::setFarPlane(float value)
{
    m_farPlane = glm::max(1e-6f, value);
    updateProjectionMatrix();
}

void PerspectiveCamera::updateProjectionMatrix()
{
     m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
}


// --- OrbitCamera ---

OrbitCamera::OrbitCamera(const std::string& name)
    : PerspectiveCamera(name)
    , m_center(0.0f)
    , m_radius(3.0f)
    , m_azimuth(0.0f)
    , m_polar(0.0f)
{
    updateViewMatrix();
}

void OrbitCamera::setRadius(float value)
{
    m_radius = glm::max(1e-6f, value);
    updateViewMatrix();
}

void OrbitCamera::setAzimuth(float value)
{
    m_azimuth = value;
    updateViewMatrix();
}

void OrbitCamera::setPolar(float value)
{
    m_polar = glm::clamp(value, glm::radians(-90.0f), glm::radians(90.0f));
    updateViewMatrix();
}

glm::vec3 OrbitCamera::getPosition()
{
    return m_center + getSpherePosition();
}

glm::vec3 OrbitCamera::getSpherePosition() const
{
    return glm::vec3(
        m_radius * glm::cos(m_polar) * glm::cos(m_azimuth),
        m_radius * glm::sin(m_polar),
        m_radius * glm::cos(m_polar) * glm::sin(m_azimuth)
    );
}

void OrbitCamera::updateViewMatrix()
{
    m_viewMatrix = glm::lookAt(getPosition(), m_center, glm::vec3(0.0f, 1.0f, 0.0f));
}


} // namespace resource
} // namespace jdl
