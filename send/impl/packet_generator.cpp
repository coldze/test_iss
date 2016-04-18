#include "packet_generator.hpp"

namespace Send
{
  PacketGenerator::PacketGenerator(std::vector<Send::PacketFactory::SharedPtr>&& packetFactories, PacketSizeGenerator&& packetSizeGenerator)
    : m_packetFactories(packetFactories)
    , m_packetSizeGenerator(packetSizeGenerator)
  {
    if (m_packetFactories.size() < 1)
    {
      throw std::runtime_error("No packet factories were provided.");
    }
    Regenerate();
  }

  std::uint32_t PacketGenerator::PlacePacketTo(char* memBuffer, const std::uint32_t availableSize)
  {
    const auto sentSize = m_packet->SendPart(memBuffer, availableSize);
    if (m_packet->IsSent())
    {
      Regenerate();
    }
    return sentSize;
  }

  void PacketGenerator::Regenerate()
  {
    const auto factoryIndex = std::rand() % m_packetFactories.size();
    m_packet = m_packetFactories.at(factoryIndex)->GeneratePacket(m_packetSizeGenerator());
  }
}
