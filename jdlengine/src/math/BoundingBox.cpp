#include "math/BoundingBox.hpp"


namespace jdl
{
namespace math
{

BoundingBox::BoundingBox()
    : m_min(1.0f)
    , m_max(-1.0f)
{}

void BoundingBox::extend(const glm::vec3& point)
{
    if (!isNull())
    {
        m_min = glm::min(m_min, point);
        m_max = glm::max(m_max, point);
    }
    else
    {
        m_min = point;
        m_max = point;
    }
}

void BoundingBox::extend(const BoundingBox& bbox)
{
    if (!bbox.isNull())
    {
        extend(bbox.m_min);
        extend(bbox.m_max);
    }
}

} // namespace math
} // namespace jdl
