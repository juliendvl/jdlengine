#pragma once

#include "size.hpp"


namespace jdl
{
namespace core
{

enum class EventType
{
	eResize,
};


class Event
{
public:
	/**
	 * @brief Returns the event type.
	 */
	EventType get_type() const { return m_type; }

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
	 * @param width Event width.
	 * @param height Event height.
	 */
	ResizeEvent(int width, int height)
		: Event(EventType::eResize)
		, m_width(width)
		, m_height(height)
	{}

	/**
	 * @brief Returns the event width.
	 */
	int get_width() const { return m_width; }

	/**
	 * @brief Returns the event height.
	 */
	int get_height() const { return m_height; }

	/**
	 * @brief Returns the event size.
	 */
	Size get_size() const { return Size(m_width, m_height); }

private:
	int m_width;
	int m_height;
};

} // namespace core
} // namespace jdl
