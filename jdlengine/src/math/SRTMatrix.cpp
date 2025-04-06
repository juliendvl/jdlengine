#include "math/SRTMatrix.hpp"

#include <glm/gtx/matrix_decompose.hpp>


namespace jdl
{
namespace math
{

SRTMatrix::SRTMatrix(const glm::vec3& scale, const glm::quat& rotation, const glm::vec3& translation)
    : scale(scale)
    , rotation(rotation)
    , translation(translation)
{}

SRTMatrix::SRTMatrix(const glm::mat4& matrix)
{
    fromMatrix(matrix);
}

void SRTMatrix::getRotation(float& outAngle, glm::vec3& outAxis)
{
    outAngle = glm::angle(rotation);
    outAxis = glm::axis(rotation);
}

void SRTMatrix::getRotation(glm::vec3& outAngles)
{
    outAngles = glm::eulerAngles(rotation);
}

void SRTMatrix::setRotation(float angle, const glm::vec3& axis)
{
    rotation = glm::angleAxis(angle, glm::normalize(axis));
}

void SRTMatrix::setRotation(const glm::vec3& angles)
{
    rotation = glm::quat(angles);
}

glm::mat4 SRTMatrix::toMatrix() const
{
    glm::vec3 skew = glm::vec3(0.0f);
    glm::vec4 perspective = glm::vec4(0.0f);

    return glm::recompose(scale, rotation, translation, skew, perspective);
}

void SRTMatrix::fromMatrix(const glm::mat4& matrix)
{
    glm::vec3 skew;
    glm::vec4 perspective;

    glm::decompose(matrix, scale, rotation, translation, skew, perspective);
}

SRTMatrix SRTMatrix::getInverse() const
{
    glm::mat4 source = toMatrix();
    return SRTMatrix(glm::inverse(source));
}

SRTMatrix& SRTMatrix::operator*=(const SRTMatrix& other)
{
    fromMatrix(toMatrix() * other.toMatrix());
    return *this;
}

SRTMatrix& SRTMatrix::operator*=(const glm::mat4& other)
{
    fromMatrix(toMatrix() * other);
    return *this;
}

SRTMatrix operator*(const SRTMatrix& a, const SRTMatrix& b)
{
    SRTMatrix result(a);
    return result *= b;
}

SRTMatrix operator*(const SRTMatrix& a, const glm::mat4& b)
{
    SRTMatrix result(a);
    return result *= b;
}

glm::mat4 operator*(const glm::mat4& a, const SRTMatrix& b)
{
    return a * b.toMatrix();
}

} // namespace math
} // namespace jdl
