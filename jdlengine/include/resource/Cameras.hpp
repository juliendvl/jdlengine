#pragma once

#include "Resource.hpp"


namespace jdl
{
namespace resource
{

class JDL_API Camera : public Resource
{
public:
    /**
     * @brief Base constructor.
     * @param name Camera name
     */
    Camera(const std::string& name) : Resource(name) {}

    /**
     * @brief Returns the camera view matrix.
     */
    const glm::mat4& getViewMatrix() const { return m_viewMatrix; }

    /**
     * @brief Returns the camera projection matrix.
     */
    const glm::mat4& getProjectionMatrix() const { return m_projectionMatrix; }

    /**
     * @brief Returns the camera position, in world space.
     */
    virtual glm::vec3 getPosition() const = 0;

protected:
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    virtual void updateViewMatrix() = 0;
    virtual void updateProjectionMatrix() = 0;
};


class JDL_API PerspectiveCamera : public Camera
{
public:
    /**
     * @brief Base constructor.
     * @param name Camera name
     */
    PerspectiveCamera(const std::string& name);

    /**
     * @brief Returns the FOV angle, in radians.
     */
    float getFOV() const { return m_fov; }

    /**
     * @brief Returns the aspect ratio value.
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
     * @param value FOV angle, in radians.
     */
    void setFOV(float value);

    /**
     * @brief Sets the aspect ratio.
     * @param value New aspect ratio.
     */
    void setAspectRatio(float value);

    /**
     * @brief Sets the near plane position.
     * @param value Near plane position.
     */
    void setNearPlane(float value);

    /**
     * @brief Sets the far plane position.
     * @param value Far plane position.
     */
    void setFarPlane(float value);

private:
    float m_fov         = glm::radians(45.0f);
    float m_aspectRatio = 1.0f;
    float m_nearPlane   = 0.1f;
    float m_farPlane    = 10000.0f;

    void updateProjectionMatrix() final;
};


class JDL_API OrbitCamera : public PerspectiveCamera
{
public:
    /**
     * @brief Creates the camera.
     * @param name Camera name
     */
    OrbitCamera(const std::string& name);

    /**
     * @brief Returns the sphere position in world space.
     */
    const glm::vec3& getCenter() const { return m_center; }

    /**
     * @brief Returns the sphere radius.
     */
    float getRadius() const { return m_radius; }

    /**
     * @brief Returns the azimuth angle (allowing to look to the left/right) in radians.
     */
    float getAzimuth() const { return m_azimuth; }

    /**
     * @brief Returns the polar angle (allowing to look up/down) in radians.
     */
    float getPolar() const { return m_polar; }

    /**
     * @brief Sets the sphere position.
     * @param value New position in world space
     */
    void setCenter(const glm::vec3& value);

    /**
     * @brief Sets the sphere radius.
     * @param value The new radius
     */
    void setRadius(float value);

    /**
     * @brief Sets the azimuth angle (allowing to look to the left/right).
     * @param value Angle in radians
     */
    void setAzimuth(float value);

    /**
     * @brief Sets the polar angle (allowing to look up/down).
     * @param value Angle in radians
     */
    void setPolar(float value);

    /**
     * @brief Returns the camera position, in world space.
     */
    glm::vec3 getPosition() const final;

private:
    // Sphere position in world space
    glm::vec3 m_center = glm::vec3(0.0f);
    // Local position on sphere
    float m_radius  = 5.0f;
    float m_azimuth = 0.0f;
    float m_polar   = 0.0f;

    void updateViewMatrix() final;
};

} // namespace resource
} // namespace jdl
