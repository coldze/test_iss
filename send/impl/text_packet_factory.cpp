#include "text_packet_factory.hpp"

#include "packet_impl.hpp"

#include <string.h>

namespace Send
{
  TextPacketFactory::TextPacketFactory(std::shared_ptr<std::vector<char>> workingBuffer, const std::string& textEnding)
    : m_packetToSend(workingBuffer)
    , m_textEnding(textEnding)
  {
    if (m_textEnding.size() < 1)
    {
      throw std::runtime_error("Invalid text-ending size.");
    }
    if (m_packetToSend.get() == nullptr)
    {
      throw std::runtime_error("Invalid buffer provided for binary-packet factory.");
    }
    if (m_packetToSend->size() < textEnding.size())
    {
      throw std::runtime_error("Packet minimal size is greater, than provided buffer");
    }
  }

  std::unique_ptr<Send::Packet> TextPacketFactory::GeneratePacket(std::uint32_t packetSizeToGenerate) const
  {
    if (m_packetToSend->size() < packetSizeToGenerate)
    {
      throw std::runtime_error("Requested to generate too large packet.");
    }
    auto ref = &m_packetToSend->front();
    memset(ref, 0, packetSizeToGenerate - 4);
    std::copy(m_textEnding.c_str(), m_textEnding.c_str() + m_textEnding.size(), ref + packetSizeToGenerate - 4);
    return std::make_unique<Send::PacketImpl>(&ref[0], packetSizeToGenerate);
  }
}
