#include "cyclic_receiver.hpp"

namespace Receive
{
  CyclicReceiver::CyclicReceiver(PacketBuildingReceiver::UniquePtr packetBuilder)
    : m_packetBuildingReceiver(std::move(packetBuilder))
  {
    if (m_packetBuildingReceiver.get() == nullptr)
    {
      throw std::runtime_error("Invalid packet builder specified.");
    }
  }

  void CyclicReceiver::Receive(const char* data, unsigned int size)
  {
    auto dataPtr = data;
    auto dataSize = size;
    auto previousDataSize = dataSize;
    for (; dataSize > 0;)
    {
      auto extractedSize = m_packetBuildingReceiver->OnPacketData(dataPtr, dataSize);
      dataSize -= extractedSize;
      dataPtr += extractedSize;
      if (dataSize == previousDataSize)
      {
        throw std::runtime_error("Cycle detected.");
      }
      previousDataSize = dataSize;
    }
  }
}
