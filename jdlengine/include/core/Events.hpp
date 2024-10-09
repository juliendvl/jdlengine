#pragma once


namespace jdl
{
namespace core
{

enum Modifiers
{
    kNone    = 0,
    kAlt     = 1,
    kControl = 2,
    kShift   = 4
};

inline Modifiers operator|(Modifiers a, Modifiers b) {
    return static_cast<Modifiers>(static_cast<int>(a) | static_cast<int>(b));
}

inline Modifiers operator&(Modifiers a, Modifiers b) {
    return static_cast<Modifiers>(static_cast<int>(a) & static_cast<int>(b));
}

inline Modifiers& operator|=(Modifiers& a, Modifiers b) {
    return a = a | b;
}

inline Modifiers& operator&=(Modifiers& a, Modifiers b) {
    return a = a & b;
}


enum class MouseButton
{
    kNoButton,
    kLeftButton,
    kRightButton,
    kMiddleButton
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
    EventType getType() const {
        return m_type;
    }

protected:
    /**
     * @brief Base constructor.
     * @param type Event type.
     */
    Event(EventType type)
        : m_type(type)
    {}

private:
    EventType m_type;
};


class MouseEvent : public Event
{
public:
    /**
     * @brief Returns the mouse X position.
     */
    int getX() const {
        return m_mouseX;
    }

    /**
     * @brief Returns the mouse Y position.
     */
    int getY() const {
        return m_mouseY;
    }

    /**
     * @brief Returns the keyboard modifiers flags.
     */
    Modifiers getModifiers() const {
        return m_modifiers;
    }

protected:
    MouseEvent(EventType type, int x, int y, Modifiers modifiers)
        : Event(type)
        , m_mouseX(x)
        , m_mouseY(y)
        , m_modifiers(modifiers)
    {}

private:
    int       m_mouseX = 0;
    int       m_mouseY = 0;
    Modifiers m_modifiers = Modifiers::kNone;
};


class MouseButtonEvent : public MouseEvent
{
public:
    /**
     * @brief Returns the button that caused the event.
     */
    MouseButton getButton() const {
        return m_button;
    }

protected:
    MouseButtonEvent(EventType type, int x, int y, MouseButton button, Modifiers modifiers)
        : MouseEvent(type, x, y, modifiers)
        , m_button(button)
    {}

private:
    MouseButton m_button = MouseButton::kNoButton;
};


class MousePressEvent : public MouseButtonEvent
{
public:
    /**
     * @brief Inits the event.
     * @param x, y Mouse position in screen space
     * @param button Pressed mouse button
     * @param modifiers Keyboard modifiers flags
     */
    MousePressEvent(int x, int y, MouseButton button, Modifiers modifiers)
        : MouseButtonEvent(EventType::kMousePress, x, y, button, modifiers)
    {}
};


class MouseMoveEvent : public MouseButtonEvent
{
public:
    /**
     * @brief Inits the event.
     * @param x, y Mouse position in screen space
     * @param button Pressed button during the movement
     * @param modifiers Keyboard modifiers flags
     */
    MouseMoveEvent(int x, int y, MouseButton button, Modifiers modifiers)
        : MouseButtonEvent(EventType::kMouseMove, x, y, button, modifiers)
    {}
};


class MouseReleaseEvent : public MouseButtonEvent
{
public:
    /**
     * @brief Inits the event.
     * @param x, y Mouse position in screen space
     * @param button Released mouse button
     * @param modifiers Keyboard modifiers flags
     */
    MouseReleaseEvent(int x, int y, MouseButton button, Modifiers modifiers)
        : MouseButtonEvent(EventType::kMouseRelease, x, y, button, modifiers)
    {}
};


class WheelEvent : public MouseEvent
{
public:
    /**
     * @brief Inits the event.
     * @param x, y Mouse position in screen space
     * @param delta Mouse wheel delta
     * @param modifiers Keyboard modifiers flags
     */
    WheelEvent(int x, int y, int delta, Modifiers modifiers)
        : MouseEvent(EventType::kWheel, x, y, modifiers)
        , m_delta(delta)
    {}

    /**
     * @brief Returns the wheel delta value.
     */
    int getDelta() const {
        return m_delta;
    }

private:
    int m_delta;
};


class ResizeEvent : public Event
{
public:
    /**
     * @brief Inits the event.
     * @param width  Event width.
     * @param height Event height.
     */
    ResizeEvent(int width, int height)
        : Event(EventType::kResize)
        , m_width(width)
        , m_height(height)
    {}

    /**
     * @brief Returns the event width.
     */
    int getWidth() const {
        return m_width;
    }

    /**
     * @brief Returns the event height.
     */
    int getHeight() const {
        return m_height;
    }

private:
    int m_width;
    int m_height;
};

} // namespace core
} // namespace jdl
