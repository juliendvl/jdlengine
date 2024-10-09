#include "resource/Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>


namespace jdl
{
namespace resource
{

// ----------------------------------------------------------------------------
// PerspectiveCamera
// ----------------------------------------------------------------------------

PerspectiveCamera::PerspectiveCamera(const std::string& name) : Camera(name)
{
    updateProjectionMatrix();
}

void PerspectiveCamera::updateProjectionMatrix()
{
    m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_nearPlane, m_farPlane);
}

void PerspectiveCamera::setFOV(float angle)
{
    m_fov = glm::clamp(angle, 0.0f, glm::pi<float>());
    updateProjectionMatrix();
}

void PerspectiveCamera::setAspectRatio(float ratio)
{
    assert(ratio > 0.0f && "Aspect ratio must be > 0!");

    m_aspectRatio = ratio;
    updateProjectionMatrix();
}

void PerspectiveCamera::setNearPlane(float near)
{
    m_nearPlane = glm::max(near, 0.001f);
    updateProjectionMatrix();
}

void PerspectiveCamera::setFarPlane(float far)
{
    m_farPlane = glm::max(far, 0.0f);
    updateProjectionMatrix();
}


// ----------------------------------------------------------------------------
// OrbitCamera
// ----------------------------------------------------------------------------

OrbitCamera::OrbitCamera(const std::string& name)
    : PerspectiveCamera(name)
{
    updateViewMatrix();
}

void OrbitCamera::setRadius(float radius)
{
    m_radius = glm::max(radius, 0.001f);
    updateViewMatrix();
}

void OrbitCamera::setTheta(float theta)
{
    m_theta = glm::clamp(theta, glm::radians(-89.9f), glm::radians(89.9f));
    updateViewMatrix();
}

void OrbitCamera::setPhi(float phi)
{
    m_phi = phi;
    updateViewMatrix();
}

void OrbitCamera::setCenter(const glm::vec3& center)
{
    m_center = center;
    updateViewMatrix();
}

glm::vec3 OrbitCamera::getOrbitPosition()
{
    return glm::vec3(
        m_radius * glm::cos(m_theta) * glm::cos(m_phi),
        m_radius * glm::sin(m_theta),
        m_radius * glm::cos(m_theta) * glm::sin(m_phi)
    );
}

void OrbitCamera::updateViewMatrix()
{
    m_viewMatrix = glm::lookAt(getPosition(), m_center, glm::vec3(0.0f, 1.0f, 0.0f));
}

} // namespace resource
} // namespace jdl
