#pragma once


namespace jdl
{
namespace utils
{

/**
 * @brief Base class deleting the copy and move operations of the derived class C.
 *
 * Inherit privately: class C : private utils::NonCopyable<C>.
 * The template parameter gives each derived class a distinct base type, so that two
 * non-copyable bases never collide (e.g. when a class derives from two of them).
 */
template <class C>
class NonCopyable
{
    public:
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable(NonCopyable&&) = delete;

        NonCopyable& operator=(const NonCopyable&) = delete;
        NonCopyable& operator=(NonCopyable&&) = delete;

    protected:
        NonCopyable() = default;
        ~NonCopyable() = default;
};

} // namespace utils
} // namespace jdl
