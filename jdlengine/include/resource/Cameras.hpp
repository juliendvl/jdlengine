#pragma once

#include "Resource.hpp"

#include <glm/glm.hpp>


namespace jdl
{
namespace resource
{

class JDL_API Camera : public Resource
{
public:
    /**
     * @brief Creates the camera.
     * @param name Camera name.
     */
    Camera(const std::string& name) : Resource(name) {}

    /**
     * @brief Returns the view matrix.
     */
    const glm::mat4& getViewMatrix() const {
        return m_viewMatrix;
    }

    /**
     * @brief Returns the projection matrix.
     */
    const glm::mat4& getProjectionMatrix() const {
        return m_projectionMatrix;
    }

    /**
     * @brief Returns the camera position in world space.
     */
    virtual glm::vec3 getPosition() const = 0;

protected:
    glm::mat4 m_viewMatrix = glm::mat4(1.0f);
    glm::mat4 m_projectionMatrix = glm::mat4(1.0f);

    virtual void updateViewMatrix() = 0;
    virtual void updateProjectionMatrix() = 0;
};


class JDL_API PerspectiveCamera : public Camera
{
public:
    /**
     * @brief Creates the camera.
     * @param name Camera name.
     */
    PerspectiveCamera(const std::string& name);

    /**
     * @brief Returns the FOV angle, in radians.
     */
    float getFOV() const { return m_fov; }

    /**
     * @brief Returns the aspect ratio.
     */
    float getAspectRatio() const { return m_aspectRatio; }

    /**
     * @brief Returns the near plane value.
     */
    float getNearPlane() const { return m_nearPlane; }

    /**
     * @brief Returns the far plane value.
     */
    float getFarPlane() const { return m_farPlane; }

    /**
     * @brief Sets the FOV angle.
     * @param angle FOV angle, in radians.
     */
    void setFOV(float angle);

    /**
     * @brief Sets the aspect ratio.
     * @param ratio Aspect ratio value.
     */
    void setAspectRatio(float ratio);

    /**
     * @brief Sets the near plane.
     * @param near Near plane value.
     */
    void setNearPlane(float near);

    /**
     * @brief Sets the far plane.
     * @param far Far plane value.
     */
    void setFarPlane(float far);

private:
    float m_fov = glm::radians(45.0f);
    float m_aspectRatio = 1.0f;
    float m_nearPlane = 0.1f;
    float m_farPlane = 10000.0f;

    void updateProjectionMatrix();
};


class JDL_API OrbitCamera : public PerspectiveCamera
{
public:
    /**
     * @brief Creates the camera.
     * @param name Camera name.
     */
    OrbitCamera(const std::string& name);

    /**
     * @brief Returns the camera position in world space.
     */
    glm::vec3 getPosition() const;

    /**
     * @brief Returns the camera target position, in world space.
     */
    const glm::vec3& getCenter() const { return m_center; }

    /**
     * @brief Returns the camera distance from the sphere center.
     */
    float getRadius() const { return m_radius; }

    /**
     * @brief Returns the camera yaw angle (horizontal rotation), in radians.
     */
    float getYaw() const { return m_yaw; }

    /**
     * @brief Returns the camera pitch angle (vertical rotation), in radians.
     */
    float getPitch() const { return m_pitch; }

    /**
     * @brief Sets the camera target position.
     * @param position New position, in world space.
     */
    void setCenter(const glm::vec3& position);

    /**
     * @brief Sets the camera distance from the sphere center.
     * @param radius New distance.
     */
    void setRadius(float radius);

    /**
     * @brief Sets the camera yaw angle (horizontal rotation).
     * @param angle New angle in radians.
     */
    void setYaw(float yaw);

    /**
     * @brief Sets the camera pitch angle (vertical rotation).
     * @param angle New angle in radians.
     */
    void setPitch(float pitch);

    /**
     * @brief Updates the camera rotation.
     * @param deltaYaw      Yaw angle increment.
     * @param deltaPitch    Pitch angle increment.
     */
    void updateRotation(float deltaYaw, float deltaPitch);

private:
    // Orbit sphere position
    glm::vec3 m_center = glm::vec3(0.0f);
    // Position in the orbit sphere
    float m_radius = 5.0f;
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;

    void updateViewMatrix();
};

} // namespace resource
} // namespace jdl
