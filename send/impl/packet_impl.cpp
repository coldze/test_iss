#include "packet_impl.hpp"

namespace Send
{
  PacketImpl::PacketImpl(const char* data, std::uint32_t dataSize)
    : m_data(data)
    , m_dataSize(dataSize)
  {
    if (m_data == nullptr)
    {
      throw std::runtime_error("Invalid packet. Data is null.");
    }
  }

  std::uint32_t PacketImpl::SendPart(char* memBuffer, const std::uint32_t availableSize)
  {
    std::uint32_t sizeToSend = (availableSize < m_dataSize) ? availableSize : m_dataSize;
    std::copy(m_data, m_data + sizeToSend, memBuffer);
    m_dataSize -= sizeToSend;
    m_data += sizeToSend;
    return sizeToSend;
  }

  bool PacketImpl::IsSent() const
  {
    return m_dataSize == 0;
  }
}
