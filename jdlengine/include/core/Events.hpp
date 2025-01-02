#pragma once

#include "Size.hpp"


namespace jdl
{
namespace core
{

enum Modifiers
{
    kNone    = 0,
    kAlt     = 1 << 0,
    kControl = 1 << 1,
    kShift   = 1 << 2
};

inline Modifiers operator|(Modifiers a, Modifiers b) { return (Modifiers)((int)(a) | (int)(b)); }
inline Modifiers operator&(Modifiers a, Modifiers b) { return (Modifiers)((int)(a) & (int)(b)); }

inline Modifiers& operator|=(Modifiers& a, Modifiers b) { return (Modifiers&)((int&)a |= (int)b); }
inline Modifiers& operator&=(Modifiers& a, Modifiers b) { return (Modifiers&)((int&)a &= (int)b); }


enum class MouseButton
{
    kNone,
    kLeft,
    kMiddle,
    kRight
};


enum class EventType
{
    kResize,
    kMousePress,
    kMouseMove,
    kMouseRelease,
    kWheel
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


class MouseEvent : public Event
{
public:
    /**
     * @brief Returns the mouse X position.
     */
    int getX() const { return m_mouseX; }

    /**
     * @brief Returns the mouse Y position.
     */
    int getY() const { return m_mouseY; }

    /**
     * @brief Returns the keyboard modifiers.
     */
    Modifiers getModifiers() const { return m_modifiers; }

protected:
    MouseEvent(EventType type, int x, int y, Modifiers modifiers)
        : Event(type)
        , m_mouseX(x)
        , m_mouseY(y)
        , m_modifiers(modifiers)
    {}

private:
    // Mouse position
    int m_mouseX;
    int m_mouseY;
    // Keyboard modifiers
    Modifiers m_modifiers;
};


class MouseButtonEvent : public MouseEvent
{
public:
    /**
     * @brief Returns the pressed button.
     */
    MouseButton getButton() const { return m_button; }

protected:
    MouseButtonEvent(EventType type, int x, int y, MouseButton button, Modifiers modifiers)
        : MouseEvent(type, x, y, modifiers)
        , m_button(button)
    {}

private:
    MouseButton m_button;
};


class MousePressEvent : public MouseButtonEvent
{
public:
    /**
     * @brief Inits the event.
     * @param x, y      Mouse X/Y position
     * @param button    Mouse button
     * @param modifiers Keyboard modifiers
     */
    MousePressEvent(int x, int y, MouseButton button, Modifiers modifiers)
        : MouseButtonEvent(EventType::kMousePress, x, y, button, modifiers)
    {}
};


class MouseReleaseEvent : public MouseButtonEvent
{
public:
    /**
     * @brief Inits the event.
     * @param x, y      Mouse X/Y position
     * @param button    Mouse button
     * @param modifiers Keyboard modifiers
     */
    MouseReleaseEvent(int x, int y, MouseButton button, Modifiers modifiers)
        : MouseButtonEvent(EventType::kMouseRelease, x, y, button, modifiers)
    {}
};


class MouseMoveEvent : public MouseButtonEvent
{
public:
    /**
     * @brief Inits the event.
     * @param x, y      Mouse X/Y position
     * @param dx, dy    Delta X/Y position
     * @param button    Mouse button
     * @param modifiers Keyboard modifiers
     */
    MouseMoveEvent(int x, int y, int dx, int dy, MouseButton button, Modifiers modifiers)
        : MouseButtonEvent(EventType::kMouseMove, x, y, button, modifiers)
        , m_deltaX(dx)
        , m_deltaY(dy)
    {}

    /**
     * @brief Returns the X delta position.
     */
    int getDeltaX() const { return m_deltaX; }

    /**
     * @brief Returns the Y delta position.
     */
    int getDeltaY() const { return m_deltaY; }

private:
    int m_deltaX;
    int m_deltaY;
};


class WheelEvent : public MouseEvent
{
public:
    /**
     * @brief Inits the event.
     * @param x, y           Mouse X/Y position
     * @param deltaX, deltaY Wheel X/Y delta
     * @param modifiers      Keyboard modifiers
     */
    WheelEvent(int x, int y, int deltaX, int deltaY, Modifiers modifiers)
        : MouseEvent(EventType::kWheel, x, y, modifiers)
        , m_deltaX(deltaX)
        , m_deltaY(deltaY)
    {}

    /**
     * @brief Returns the wheel X delta.
     */
    int getDeltaX() const { return m_deltaX; }

    /**
     * @brief Returns the wheel Y delta.
     */
    int getDeltaY() const { return m_deltaY; }

private:
    int m_deltaX;
    int m_deltaY;
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
     * @brief Returns the event width.
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
