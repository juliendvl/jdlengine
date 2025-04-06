#pragma once

#include "core/Core.hpp"

#include <glm/gtx/quaternion.hpp>


namespace jdl
{
namespace math
{

struct JDL_API SRTMatrix
{
    // SRT Components
    glm::vec3 scale       = glm::vec3(1.0f);
    glm::quat rotation    = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 translation = glm::vec3(0.0f);

    /**
     * @brief Creates an identity SRT matrix.
     */
    SRTMatrix() = default;

    /**
     * @brief Creates a SRT matrix from the components.
     * @param scale       Scale component
     * @param rotation    Rotation component (quaternion)
     * @param translation Translation component
     */
    SRTMatrix(const glm::vec3& scale, const glm::quat& rotation, const glm::vec3& translation);

    /**
     * @brief Creates a SRT matrix from an input 4x4 matrix.
     * @param matrix Source 4x4 matrix
     */
    SRTMatrix(const glm::mat4& matrix);

    /**
     * @brief Returns the rotation component (angle-axis representation).
     * @param outAngle Output angle in radians
     * @param outAxis  Output normalized axis
     */
    void getRotation(float& outAngle, glm::vec3& outAxis);

    /**
     * @brief Returns the rotation component (Euler angles representation).
     * @param outAngles Output angles in radians
     */
    void getRotation(glm::vec3& outAngles);

    /**
     * @brief Sets the rotation component (angle-axis representation).
     * @param angle Angle in radians
     * @param axis  Rotation axis
     */
    void setRotation(float angle, const glm::vec3& axis);

    /**
     * @brief Sets the rotation component (Euler angles representation).
     * @param angles Angles in radians
     */
    void setRotation(const glm::vec3& angles);

    /**
     * @brief Converts the SRT components to a 4x4 matrix.
     */
    glm::mat4 toMatrix() const;

    /**
     * @brief Sets the SRT components from the input 4x4 matrix.
     * @param matrix Source 4x4 matrix
     */
    void fromMatrix(const glm::mat4& matrix);

    /**
     * @brief Returns the inverse SRT matrix.
     */
    SRTMatrix getInverse() const;

    SRTMatrix& operator*=(const SRTMatrix& other);
    SRTMatrix& operator*=(const glm::mat4& other);
};

SRTMatrix operator*(const SRTMatrix& a, const SRTMatrix& b);
SRTMatrix operator*(const SRTMatrix& a, const glm::mat4& b);
glm::mat4 operator*(const glm::mat4& a, const SRTMatrix& b);

} // namespace math
} // namespace jdl
