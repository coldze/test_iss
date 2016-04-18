#include "binary_packet_factory.hpp"

#include "packet_impl.hpp"

namespace Send
{
  BinaryPacketFactory::BinaryPacketFactory(std::shared_ptr<std::vector<char>> workingBuffer, char headingChar)
    : m_packetToSend(workingBuffer)
    , m_headingChar(headingChar)
  {
    if (m_packetToSend.get() == nullptr)
    {
      throw std::runtime_error("Invalid buffer provided for binary-packet factory.");
    }
    if (m_packetToSend->size() < 5)
    {
      throw std::runtime_error("Packet minimal size is greater, than provided buffer");
    }
  }

  std::unique_ptr<Packet> BinaryPacketFactory::GeneratePacket(std::uint32_t packetSizeToGenerate) const
  {
    if (m_packetToSend->size() < packetSizeToGenerate)
    {
      throw std::runtime_error("Requested to generate too large packet.");
    }

    auto& ref = *m_packetToSend;
    std::uint32_t packetSizeWithoutHeader = packetSizeToGenerate - 5;
    ref[0] = m_headingChar;
    auto sizePtr = reinterpret_cast<char*>(&packetSizeWithoutHeader);
    std::copy(sizePtr, sizePtr + 4, &ref[1]);
    return std::make_unique<PacketImpl>(&ref[0], packetSizeToGenerate);
  }
}
