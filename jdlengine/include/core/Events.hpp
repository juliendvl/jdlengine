#pragma once

#include "Size.hpp"


namespace jdl
{
namespace core
{

enum class EventType
{
    kResize
};


class Event
{
public:
    /**
     * @brief Returns the event type.
     */
    EventType getType() const { return m_type; }

protected:
    Event(EventType type) : m_type(type) {}

private:
    EventType m_type;
};


class ResizeEvent : public Event
{
public:
    /**
     * @brief Inits the event.
     * @param width  Event width
     * @param height Event height
     */
    ResizeEvent(int width, int height)
        : Event(EventType::kResize)
        , m_width(width)
        , m_height(height)
    {}

    /**
     * @brief Retuns the event width.
     */
    int getWidth() const { return m_width; }

    /**
     * @brief Returns the event height.
     */
    int getHeight() const { return m_height; }

    /**
     * @brief Returns the event size.
     */
    Size getSize() const { return Size(m_width, m_height); }

private:
    int m_width;
    int m_height;
};

} // namespace core
} // namespace jdl
