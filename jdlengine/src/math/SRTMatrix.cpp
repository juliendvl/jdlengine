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
    fromMatrix(matrix);
}

void SRTMatrix::getRotation(glm::vec3& axis, float& angle)
{
    axis = glm::axis(rotation);
    angle = glm::angle(rotation);
}

void SRTMatrix::getRotation(glm::vec3& angles)
{
    angles = glm::eulerAngles(rotation);
}

void SRTMatrix::setRotation(const glm::vec3& axis, float angle)
{
    rotation = glm::angleAxis(angle, glm::normalize(axis));
}

glm::mat4 SRTMatrix::toMatrix() const
{
    glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 R = glm::toMat4(rotation);
    glm::mat4 T = glm::translate(glm::mat4(1.0f), translation);

    return T * R * S;
}

void SRTMatrix::fromMatrix(const glm::mat4& matrix)
{
    glm::vec3 skew;
    glm::vec4 perspective;

    glm::decompose(matrix, scale, rotation, translation, skew, perspective);
}

SRTMatrix SRTMatrix::inverse() const
{
    glm::mat4 matrix = toMatrix();
    return SRTMatrix(glm::inverse(matrix));
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
