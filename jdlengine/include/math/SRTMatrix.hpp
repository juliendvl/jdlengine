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
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;

    /**
     * @brief Creates an identity SRT matrix.
     */
    SRTMatrix();

    /**
     * @brief Creates a SRT matrix from its components.
     * @param scale       Scale component
     * @param rotation    Rotation component
     * @param translation Translation component
     */
    SRTMatrix(const glm::vec3& scale, const glm::quat& rotation, const glm::vec3& translation);

    /**
     * @brief Creates a SRT matrix from a 4x4 transform matrix.
     * @param matrix Input 4x4 matrix
     */
    SRTMatrix(const glm::mat4& matrix);

    /**
     * @brief Returns the rotation component (axis-angle representation).
     * @param axis  Output normalized axis
     * @param angle Output angle in radians
     */
    void getRotation(glm::vec3& axis, float& angle);

    /**
     * @brief Returns the rotation component (Euler angles representation).
     * @param angles Euler angles in radians
     */
    void getRotation(glm::vec3& angles);

    /**
     * @brief Sets the rotation component (axis-angle representation).
     * @param axis  Rotation axis
     * @param angle Rotation angle in radians
     */
    void setRotation(const glm::vec3& axis, float angle);

    /**
     * @brief Sets the rotation component (Euler angles representation).
     * @param angles Euler angles in radians
     */
    void setRotation(const glm::vec3& angles);

    /**
     * @brief Converts the SRT components into a 4x4 transform matrix.
     */
    glm::mat4 toMatrix() const;

    /**
     * @brief Sets the SRT components from the input matrix.
     * @param matrix Input 4x4 matrix
     */
    void fromMatrix(const glm::mat4& matrix);

    /**
     * @brief Computes and returns the inverse SRT matrix.
     */
    SRTMatrix inverse() const;

    SRTMatrix& operator*=(const SRTMatrix& other);
    SRTMatrix& operator*=(const glm::mat4& other);
};

SRTMatrix operator*(const SRTMatrix& a, const SRTMatrix& b);
SRTMatrix operator*(const SRTMatrix& a, const glm::mat4& b);
glm::mat4 operator*(const glm::mat4& a, const SRTMatrix& b);

} // namespace math
} // namespace jdl
