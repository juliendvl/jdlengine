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
    Camera(const std::string& name)
        : Resource(name)
    {}

    /**
     * @brief Returns the camera view matrix.
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
     * @brief Returns the camera position, in world space.
     */
    virtual glm::vec3 getPosition() = 0;

protected:
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    // Updates the view matrix.
    virtual void updateViewMatrix() = 0;
    // Updates the projection matrix
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
     * @brief Returns the FOV angle, in radians.
     */
    float getFOV() const {
        return m_fov;
    }

    /**
     * @brief Returns the aspect ratio.
     */
    float getAspectRatio() const {
        return m_aspectRatio;
    }

    /**
     * @brief Returns the near plane value.
     */
    float getNearPlane() const {
        return m_nearPlane;
    }

    /**
     * @brief Returns the far plane value.
     */
    float getFarPlane() const {
        return m_farPlane;
    }

    /**
     * @brief Sets the FOV.
     * @param angle FOV angle in radians
     */
    void setFOV(float angle);

    /**
     * @brief Sets the aspect ratio.
     * @param ratio Aspect ratio
     */
    void setAspectRatio(float ratio);

    /**
     * @brief Sets the near plane value.
     * @param near Near plane value
     */
    void setNearPlane(float near);

    /**
     * @brief Sets the far plane value.
     * @param far Far plane value
     */
    void setFarPlane(float far);

private:
    float m_fov         = glm::radians(45.0f);
    float m_aspectRatio = 1.0f;
    float m_nearPlane   = 0.1f;
    float m_farPlane    = 10000.0f;

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
     * @brief Returns the camera position, in world space.
     */
    glm::vec3 getPosition() {
        return m_center + getOrbitPosition();
    }

    /**
     * @brief Returns the orbit sphere radius.
     */
    float getRadius() const {
        return m_radius;
    }

    /**
     * @brief Returns the camera "theta" angle (the vertical angle on the sphere), in radians.
     */
    float getTheta() const {
        return m_theta;
    }

    /**
     * @brief Returns the camera "phi" angle (the horizontal angle on the sphere), in radians.
     */
    float getPhi() const {
        return m_phi;
    }

    /**
     * @brief Returns the orbit sphere center position, in world space.
     */
    const glm::vec3& getCenter() const {
        return m_center;
    }

    /**
     * @brief Sets the orbit sphere radius.
     * @param radius The new radius
     */
    void setRadius(float radius);

    /**
     * @brief Sets the camera "theta" angle (the vertical angle on the sphere).
     * @param theta The new angle in radians
     */
    void setTheta(float theta);

    /**
     * @brief Sets the camera "phi" angle (the horizontal angle on the sphere).
     * @param phi The new angle in radians
     */
    void setPhi(float phi);

    /**
     * @brief Sets the orbit sphere center position.
     * @param center The center new position, in world space
     */
    void setCenter(const glm::vec3& center);

private:
    // Sphere center position in world space
    glm::vec3 m_center = glm::vec3(0.0f);
    // Camera position on the orbit sphere (spherical coordinates)
    float m_radius     = 5.0f;
    float m_theta      = 0.0f;
    float m_phi        = glm::radians(90.0f);

    // Returns the position of the camera on the orbit sphere
    glm::vec3 getOrbitPosition();

    void updateViewMatrix();
};

} // namespace resource
} // namespace jdl
