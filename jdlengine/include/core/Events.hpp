#pragma once

#include "Size.hpp"


namespace jdl
{
namespace core
{

enum class EventType
{
    eMousePress,
    eMouseRelease,
    eMouseMove,
    eMouseWheel,
    eResize
};


enum class MouseButton
{
    eNone,
    eLeftButton,
    eRightButton,
    eMiddleButton
};


enum Modifiers
{
    eNone    = 0,
    eAlt     = 1 << 0,
    eControl = 1 << 1,
    eShift   = 1 << 2
};

inline Modifiers operator&(Modifiers a, Modifiers b) { return (Modifiers)((int)a & (int)b); }
inline Modifiers operator|(Modifiers a, Modifiers b) { return (Modifiers)((int)a | (int)b); }

inline Modifiers& operator&=(Modifiers& a, Modifiers b)
{
    a = a & b;
    return a;
}

inline Modifiers& operator|=(Modifiers& a, Modifiers b)
{
    a = a | b;
    return a;
}


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
     * @brief Returns the mouse X position at the time of event.
     */
    int getX() const { return m_mouseX; }

    /**
     * @brief Returns the mouse Y position at the time of event.
     */
    int getY() const { return m_mouseY; }

    /**
     * @brief Returns the keyboard modifiers at the time of event.
     */
    Modifiers getModifiers() const { return m_modifiers; }

protected:
    MouseEvent(EventType type, int mouseX, int mouseY, Modifiers modifiers)
        : Event(type)
        , m_mouseX(mouseX)
        , m_mouseY(mouseY)
        , m_modifiers(modifiers)
    {}

private:
    // Mouse position at the time of event.
    int m_mouseX = -1;
    int m_mouseY = -1;
    // Keyboard modifiers
    Modifiers m_modifiers = Modifiers::eNone;
};


class MouseButtonEvent : public MouseEvent
{
public:
    /**
     * @brief Returns the pressed button at the time of event.
     */
    MouseButton getButton() const { return m_button; }

protected:
    MouseButtonEvent(EventType type, MouseButton button, int mouseX, int mouseY, Modifiers modifiers)
        : MouseEvent(type, mouseX, mouseY, modifiers)
        , m_button(button)
    {}

private:
    MouseButton m_button = MouseButton::eNone;
};


class MousePressEvent : public MouseButtonEvent
{
public:
    /**
     * @brief Creates the event.
     * @param button Pressed button at the time of event.
     * @param mouseX    Mouse X position at the time of event.
     * @param mouseY    Mouse Y position at the time of event.
     * @param modifiers Keyboard modifiers.
     */
    MousePressEvent(MouseButton button, int mouseX, int mouseY, Modifiers modifiers)
        : MouseButtonEvent(EventType::eMousePress, button, mouseX, mouseY, modifiers)
    {}
};


class MouseReleaseEvent : public MouseButtonEvent
{
public:
    /**
     * @brief Creates the event.
     * @param button Pressed button at the time of event.
     * @param mouseX    Mouse X position at the time of event.
     * @param mouseY    Mouse Y position at the time of event.
     * @param modifiers Keyboard modifiers.
     */
    MouseReleaseEvent(MouseButton button, int mouseX, int mouseY, Modifiers modifiers)
        : MouseButtonEvent(EventType::eMouseRelease, button, mouseX, mouseY, modifiers)
    {}
};


class MouseMoveEvent : public MouseButtonEvent
{
public:
    /**
     * @brief Creates the event.
     * @param button Pressed button at the time of event.
     * @param mouseX    Mouse X position at the time of event.
     * @param mouseY    Mouse Y position at the time of event.
     * @param deltaX    Mouse X delta.
     * @param deltaY    Mouse Y delta.
     * @param modifiers Keyboard modifiers.
     */
    MouseMoveEvent(MouseButton button, int mouseX, int mouseY, int deltaX, int deltaY, Modifiers modifiers)
        : MouseButtonEvent(EventType::eMousePress, button, mouseX, mouseY, modifiers)
        , m_deltaX(deltaX)
        , m_deltaY(deltaY)
    {}

    /**
     * @brief Returns the mouse X delta.
     */
    int getDeltaX() const { return m_deltaX; }

    /**
     * @brief Returns the mouse Y delta.
     */
    int getDeltaY() const { return m_deltaY; }

private:
    int m_deltaX = -1;
    int m_deltaY = -1;
};


class WheelEvent : public MouseEvent
{
public:
    /**
     * @brief Creates the event.
     * @param mouseX    Mouse X position at the time of event.
     * @param mouseY    Mouse Y position at the time of event.
     * @param deltaX    Wheel X offset.
     * @param deltaY    Wheel Y offset.
     * @param modifiers Keyboard modifiers.
     */
    WheelEvent(int mouseX, int mouseY, int deltaX, int deltaY, Modifiers modifiers)
        : MouseEvent(EventType::eMouseWheel, mouseX, mouseY, modifiers)
        , m_deltaX(deltaX)
        , m_deltaY(deltaY)
    {}

    /**
     * @brief Returns the wheel X offset.
     */
    int getDeltaX() const { return m_deltaX; }

    /**
     * @brief Returns the wheel Y offset.
     */
    int getDeltaY() const { return m_deltaY; }

private:
    int m_deltaX = -1;
    int m_deltaY = -1;
};


class ResizeEvent : public Event
{
public:
    /**
     * @brief Creates the event.
     * @param width     Event width.
     * @param height    Event height.
     */
    ResizeEvent(int width, int height)
        : Event(EventType::eResize)
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
