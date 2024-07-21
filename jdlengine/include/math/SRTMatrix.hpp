#pragma once

#include "core/Core.hpp"

#include <glm/gtx/quaternion.hpp>


namespace jdl
{
namespace math
{

struct JDL_API SRTMatrix
{
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;

    /**
     * @brief Creates an identity SRT matrix.
     */
    SRTMatrix();

    /**
     * @brief Creates a SRT matrix.
     * @param scale       Scale component
     * @param rotation    Rotation component
     * @param translation Translation component
     */
    SRTMatrix(const glm::vec3& scale, const glm::quat& rotation, const glm::vec3& translation);

    /**
     * @brief Creates a SRT matrix.
     * @param matrix 4x4 transformation matrix
     */
    SRTMatrix(const glm::mat4& matrix);

    /**
     * @brief Returns the rotation component (axis-angle representation).
     * @param outAxis  Output rotation axis
     * @param outAngle Output rotation angle in radians
     */
    void getRotation(glm::vec3& outAxis, float& outAngle) const;

    /**
     * @brief Returns the rotation component (Euler angles representation).
     * @param outAngles Output Euler angles
     */
    void getRotation(glm::vec3& outAngles) const;

    /**
     * @brief Sets the rotation component (axis-angle representation).
     * @param axis  Rotation axis
     * @param angle Rotation angle in radians
     */
    void setRotation(const glm::vec3& axis, float angle);

    /**
     * @brief Sets the rotation component (Euler angles representation).
     * @param angles Euler angles
     */
    void setRotation(const glm::vec3& angles);

    /**
     * @brief Converts the SRT components into a 4x4 transformation matrix.
     */
    glm::mat4 toMatrix() const;

    /**
     * @brief Sets the SRT components from the input transformation matrix.
     * @param matrix 4x4 transformation matrix
     */
    void setMatrix(const glm::mat4& matrix);

    /**
     * @brief Returns the inverse SRT matrix.
     */
    SRTMatrix inverse() const;

    /**
     * @brief Multiplication operators.
     */
    SRTMatrix& operator*=(const SRTMatrix& other);
    SRTMatrix& operator*=(const glm::mat4& other);
};

SRTMatrix operator*(const SRTMatrix& m1, const SRTMatrix& m2);
SRTMatrix operator*(const SRTMatrix& m1, const glm::mat4& m2);
glm::mat4 operator*(const glm::mat4& m1, const SRTMatrix& m2);

} // namespace math
} // namespace jdl
