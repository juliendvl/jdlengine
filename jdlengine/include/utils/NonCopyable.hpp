#pragma once


namespace jdl
{
namespace utils
{

template<class C>
class NonCopyable
{
public:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&)      = delete;

    C& operator=(const C&) = delete;
    C& operator=(C&&)      = delete;

protected:
     NonCopyable() = default;
    ~NonCopyable() = default;
};

} // namespace utils
} // namespace jdl
