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
     * @brief Creates the camera.
     * @param name Camera name
     */
    Camera(const std::string& name) : Resource(name) {}

    /**
     * @brief Returns the view matrix.
     */
    const glm::mat4& getViewMatrix() const { return m_viewMatrix; }

    /**
     * @brief Returns the projection matrix.
     */
    const glm::mat4& getProjectionMatrix() const { return m_projectionMatrix; }

    /**
     * @brief Returns the camera position in world space.
     */
    virtual glm::vec3 getPosition() = 0;

protected:
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

private:
    // Updates the view/projection matrices
    virtual void updateViewMatrix() = 0;
    virtual void updateProjectionMatrix() = 0;
};


class JDL_API PerspectiveCamera : public Camera
{
public:
    /**
     * @brief Creates the camera.
     * @param name Camera name
     */
    PerspectiveCamera(const std::string& name);

    /**
     * @brief Returns the FOV angle in radians.
     */
    float getFOV() const { return m_fov; }

    /**
     * @brief Returns the aspect ratio value;
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
     * @param value FOV angle in radians
     */
    void setFOV(float value);

    /**
     * @brief Sets the aspect ratio value.
     * @param value Aspect ratio
     */
    void setAspectRatio(float value);

    /**
     * @brief Sets the near plane value.
     * @param value Near plane
     */
    void setNearPlane(float value);

    /**
     * @brief Sets the far plane value.
     * @param value Far plane
     */
    void setFarPlane(float value);

private:
    float m_fov;
    float m_aspectRatio;
    float m_nearPlane;
    float m_farPlane;

    void updateProjectionMatrix();
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
     * @brief Returns the camera sphere center position in world space.
     */
    const glm::vec3& getCenter() const { return m_center; }

    /**
     * @brief Returns the camera sphere radius.
     */
    float getRadius() const { return m_radius; }

    /**
     * @brief Returns the azimuth angle (allowing to turn around the Y axis) in radians.
     */
    float getAzimuth() const { return m_azimuth; }

    /**
     * @brief Returns the polar angle (allowing to look up and down) in radians.
     */
    float getPolar() const { return m_polar; }

    /**
     * @brief Sets the sphere radius.
     * @param value The new radius
     */
    void setRadius(float value);

    /**
     * @brief Sets the azimuth angle (allowing to turn around the Y axis).
     * @param value New angle in radians
     */
    void setAzimuth(float value);

    /**
     * @brief Sets the polar angle (allowing to look up and down).
     * @param value New angle in radians
     */
    void setPolar(float value);

    /**
     * @brief Returns the camera position in world space.
     */
    glm::vec3 getPosition();

private:
    // Sphere center position in world space
    glm::vec3 m_center;
    // Camera coordinates on the sphere
    float m_radius;
    float m_azimuth;
    float m_polar;

    // Returns the local camera position on the sphere
    glm::vec3 getSpherePosition() const;

    void updateViewMatrix();
};

} // namespace resource
} // namespace jdl
