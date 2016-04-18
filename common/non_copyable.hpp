#ifndef COMMON__NON_COPYABLE_HPP
#define COMMON__NON_COPYABLE_HPP

namespace Common
{
  class NonCopyable
  {
  public:
    virtual ~NonCopyable() {}

  protected:
    constexpr NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator = (const NonCopyable&) = delete;
  };
}

#endif