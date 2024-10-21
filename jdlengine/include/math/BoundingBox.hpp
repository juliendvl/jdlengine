#pragma once

#include "core/Core.hpp"


namespace jdl
{
namespace math
{

class JDL_API BoundingBox
{
public:
    /**
     * @brief Creates an empty bounding box.
     */
    BoundingBox();

    /**
     * @brief Returns whether the bounding box is null (i.e. not set) or not.
     */
    bool isNull() const {
        return m_min.x > m_max.x || m_min.y > m_max.y || m_min.z > m_max.z;
    }

    /**
     * @brief Extends the bounding box to include the input point.
     * @param point The point to be added
     */
    void extend(const glm::vec3& point);

    /**
     * @brief Extends the bounding box to include the input point sequence.
     * @param start Points sequence start
     * @param end Points sequence end
     */
    template<typename Iter>
    void extend(Iter start, Iter end)
    {
        for (auto it = start; it != end; ++it)
        {
            extend(*it);
        }
    }

    /**
     * @brief Extends the bounding box to include the input point sequence.
     * @param points Points array
     */
    void extend(const std::vector<glm::vec3>& points) {
        extend(points.cbegin(), points.cend());
    }

    /**
     * @brief Extends the bounding box to include the input source bounding box.
     * @param bbox Source bounding box.
     */
    void extend(const BoundingBox& bbox);

    /**
     * @brief Returns the bounding box min point.
     */
    const glm::vec3& getMin() const {
        return m_min;
    }

    /**
     * @brief Returns the bounding box max point.
     */
    const glm::vec3& getMax() const {
        return m_max;
    }

    /**
     * @brief Returns the bounding box center.
     */
    glm::vec3 getCenter() const {
        return !isNull() ? (m_min + m_max) / 2.0f : glm::vec3(0.0f);
    }

    /**
     * @brief Returns the bounding box width.
     */
    float getWidth() const {
        return glm::abs(m_max.x - m_min.x);
    }

    /**
     * @brief Returns the bounding box height.
     */
    float getHeight() const {
        return glm::abs(m_max.y - m_min.y);
    }

    /**
     * @brief Returns the bounding box depth.
     */
    float getDepth() const {
        return glm::abs(m_max.z - m_min.z);
    }

private:
    glm::vec3 m_min;
    glm::vec3 m_max;
};

} // namespace math
} // namespace jdl
