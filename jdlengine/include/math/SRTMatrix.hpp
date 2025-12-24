#pragma once

#include "core/Core.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


namespace jdl
{
namespace math
{

struct JDL_API SRTMatrix
{
    // SRT Components
    glm::vec3 scale = glm::vec3(1.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 translation = glm::vec3(0.0f);

    /**
     * @brief Creates an identity SRT matrix.
     */
    SRTMatrix() = default;

    /**
     * @brief Creates a SRT matrix from its components.
     * @param scale         Scale component.
     * @param rotation      Rotation component (quaternion).
     * @param translation   Translation component.
     */
    SRTMatrix(const glm::vec3& scale, const glm::quat& rotation, const glm::vec3& translation);

    /**
     * Creates a SRT matrix from a 4x4 transform matrix.
     * @param matrix Source matrix.
     */
    SRTMatrix(const glm::mat4& matrix);

    /**
     * @brief Returns the rotation component (axis-angle representation).
     * @param outAxis   Output normalized rotation axis.
     * @param outAngle  Output rotation angle, in radians.
     */
    void getRotation(glm::vec3& outAxis, float& outAngle) const;

    /**
     * @brief Returns the rotation component (Euler angles representation).
     * @param outAngles Output rotation angles, in radians.
     */
    void getRotation(glm::vec3& outAngles) const;

    /**
     * @brief Sets the rotation component (axis-angle representation).
     * @param axis  Rotation axis.
     * @param angle Rotation angle, in radians.
     */
    void setRotation(const glm::vec3& axis, float angle);

    /**
     * @brief Sets the rotation component (Euler angles representation).
     * @param angles Rotation angles, in radians.
     */
    void setRotation(const glm::vec3& angles);

    /**
     * @brief Converts the SRT components into a 4x4 transform matrix.
     */
    glm::mat4 toMatrix() const;

    /**
     * @brief Sets the SRT components from a 4x4 transform matrix.
     * @param matrix Source matrix.
     */
    void fromMatrix(const glm::mat4& matrix);

    /**
     * @brief Returns the inverse SRT matrix.
     */
    SRTMatrix getInverse() const {
        return SRTMatrix(glm::inverse(toMatrix()));
    }

    SRTMatrix& operator*=(const SRTMatrix& other);
    SRTMatrix& operator*=(const glm::mat4& other);
};

SRTMatrix operator*(const SRTMatrix& a, const SRTMatrix& b);
SRTMatrix operator*(const SRTMatrix& a, const glm::mat4& b);
glm::mat4 operator*(const glm::mat4& a, const SRTMatrix& b);

} // namespace math
} // namespace jdl
