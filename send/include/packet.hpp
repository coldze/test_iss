#ifndef SEND__INCLUDE__PACKET_HPP
#define SEND__INCLUDE__PACKET_HPP

#include <common/non_copyable.hpp>

#include <memory>

namespace Send
{
  
  class Packet : private Common::NonCopyable
  {
  public:
    using UniquePtr = std::unique_ptr<Packet>;

    virtual std::uint32_t SendPart(char* memBuffer, const std::uint32_t availableSize) = 0;
    virtual bool IsSent() const = 0;

  private:
    const char* m_data;
    std::uint32_t m_dataSize;
  };
}

#endif