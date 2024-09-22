#include "math/SRTMatrix.hpp"

#include <glm/gtx/matrix_decompose.hpp>


namespace jdl
{
namespace math
{

SRTMatrix::SRTMatrix()
    : scale(1.0f)
    , rotation(1.0f, 0.0f, 0.0f, 0.0f)
    , translation(0.0f)
{}

SRTMatrix::SRTMatrix(const glm::vec3& scale, const glm::quat& rotation, const glm::vec3& translation)
    : scale(scale)
    , rotation(rotation)
    , translation(translation)
{}

SRTMatrix::SRTMatrix(const glm::mat4& matrix)
    : scale(1.0f)
    , rotation(1.0f, 0.0f, 0.0f, 0.0f)
    , translation(0.0f)
{
    setMatrix(matrix);
}

void SRTMatrix::getRotation(glm::vec3& outAxis, float& outAngle) const
{
    outAxis = glm::axis(rotation);
    outAngle = glm::angle(rotation);
}

void SRTMatrix::getRotation(glm::vec3& outAngles) const
{
    outAngles = glm::eulerAngles(rotation);
}

void SRTMatrix::setRotation(const glm::vec3& axis, float angle)
{
    rotation = glm::angleAxis(angle, glm::normalize(axis));
}

void SRTMatrix::setRotation(const glm::vec3& angles)
{
    rotation = glm::quat(angles);
}

glm::mat4 SRTMatrix::toMatrix() const
{
    glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 R = glm::toMat4(rotation);
    glm::mat4 T = glm::translate(glm::mat4(1.0f), translation);

    return T * R * S;
}

void SRTMatrix::setMatrix(const glm::mat4& matrix)
{
    glm::vec3 skew;
    glm::vec4 perspective;

    glm::decompose(matrix, scale, rotation, translation, skew, perspective);
}

SRTMatrix SRTMatrix::inverse() const
{
    return SRTMatrix(glm::inverse(toMatrix()));
}

SRTMatrix& SRTMatrix::operator*=(const SRTMatrix & other)
{
    setMatrix(toMatrix() * other.toMatrix());
    return *this;
}

SRTMatrix& SRTMatrix::operator*=(const glm::mat4& other)
{
    setMatrix(toMatrix() * other);
    return *this;
}

SRTMatrix operator*(const SRTMatrix& m1, const SRTMatrix& m2)
{
    SRTMatrix res(m1);
    return res *= m2;
}

SRTMatrix operator*(const SRTMatrix& m1, const glm::mat4& m2)
{
    SRTMatrix res(m1);
    return res *= m2;
}

glm::mat4 operator*(const glm::mat4& m1, const SRTMatrix& m2)
{
    return m1 * m2.toMatrix();
}

} // namespace math
} // namespace jdl
