#ifndef SEND__IMPL__TEXT_PACKET_FACTORY_HPP
#define SEND__IMPL__TEXT_PACKET_FACTORY_HPP

#include "send/include/packet_factory.hpp"

#include <memory>
#include <string>
#include <vector>

namespace Send
{
  class TextPacketFactory final : public Send::PacketFactory
  {
  public:
    TextPacketFactory(std::shared_ptr<std::vector<char>> workingBuffer, const std::string& textEnding);
    
  protected:
    std::unique_ptr<Send::Packet> GeneratePacket(std::uint32_t packetSizeToGenerate) const override;
    
  private:
    const std::shared_ptr<std::vector<char>> m_packetToSend;
    const std::string m_textEnding;
  };
}

#endif