#ifndef SEND__IMPL__PACKET_IMPL_HPP
#define SEND__IMPL__PACKET_IMPL_HPP

#include "send/include/packet.hpp"

#include <memory>

namespace Send
{
  class PacketImpl final : public Packet
  {
  public:
    using UniquePtr = std::unique_ptr<Packet>;

    PacketImpl(const char* data, std::uint32_t dataSize);

  protected:
    std::uint32_t SendPart(char* memBuffer, const std::uint32_t availableSize) override;
    bool IsSent() const override;

  private:
    const char* m_data;
    std::uint32_t m_dataSize;
  };
}

#endif