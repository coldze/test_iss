#ifndef SEND__IMPL__BINARY_PACKET_FACTORY_HPP
#define SEND__IMPL__BINARY_PACKET_FACTORY_HPP

#include "send/include/packet_factory.hpp"

#include <memory>
#include <vector>

namespace Send
{
  class BinaryPacketFactory final : public PacketFactory
  {
  public:
    BinaryPacketFactory(std::shared_ptr<std::vector<char>> workingBuffer, char headingChar);

  protected:
    std::unique_ptr<Packet> GeneratePacket(std::uint32_t packetSizeToGenerate) const override;

  private:
    const std::shared_ptr<std::vector<char>> m_packetToSend;
    const char m_headingChar;
  };
}

#endif