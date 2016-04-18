#include "text_and_binary_packet_receiver.hpp"

#include <algorithm>

namespace Receive
{
  TextAndBinaryPacketReceiver::TextAndBinaryPacketReceiver(std::uint32_t bufferSize, char binaryHeader, const std::string& textEnding, ReceivedPacketCallback binaryCallback, ReceivedPacketCallback textCallback)
    : m_binaryHeader(binaryHeader)
    , m_textEnd(textEnding)
    , m_onBinaryPacketDone(binaryCallback)
    , m_onTextPacketDone(textCallback)
    , m_headerSize(0)
    , m_buffer(std::make_unique<std::vector<char>>(bufferSize, 0))
  {
    m_currentPtr = &(*m_buffer)[0];
    m_headerBuffer = reinterpret_cast<char*>(&m_expectedSize);
    m_onNewPacket = [this](const char* memBuffer, const std::uint32_t availableSize) -> std::uint32_t
    {
      return this->OnNewPacket(memBuffer, availableSize);
    };
    m_onBinaryHeader = [this](const char* memBuffer, const std::uint32_t availableSize) -> std::uint32_t
    {
      return this->OnBinaryPacketHeader(memBuffer, availableSize);
    };
    m_onBinaryData = [this](const char* memBuffer, const std::uint32_t availableSize) -> std::uint32_t
    {
      return this->OnBinaryPacketData(memBuffer, availableSize);
    };
    m_onTextData = [this](const char* memBuffer, const std::uint32_t availableSize) -> std::uint32_t
    {
      return this->OnTextPacketData(memBuffer, availableSize);
    };
    m_packetDataProcessor = &m_onNewPacket;
  }
  
  std::uint32_t TextAndBinaryPacketReceiver::OnPacketData(const char* memBuffer, const std::uint32_t availableSize)
  {
    if (availableSize <= 0)
    {
      return 0;
    }
    return (*m_packetDataProcessor)(memBuffer, availableSize);
  }
  
  std::uint32_t TextAndBinaryPacketReceiver::OnNewPacket(const char* memBuffer, const std::uint32_t availableSize)
  {
    m_currentPtr = &m_buffer->front();
    if (m_binaryHeader == memBuffer[0])
    {
      m_headerSize = 0;
      m_packetDataProcessor = &m_onBinaryHeader;
      return 1 + m_onBinaryHeader(memBuffer + 1, availableSize - 1);
    }
    m_currentSize = 0;
    m_packetDataProcessor = &m_onTextData;
    return m_onTextData(memBuffer, availableSize);
  }
  
  std::uint32_t TextAndBinaryPacketReceiver::OnTextPacketData(const char* memBuffer, const std::uint32_t availableSize)
  {
    const std::uint32_t copyFromLeft = (m_currentSize > 2) ? 3 : m_currentSize;
    if (copyFromLeft && ((4 - copyFromLeft) <= availableSize))
    {
      const std::uint32_t copyFromRight = availableSize > 3 ? 3 : availableSize;
      const std::string leftSide(m_currentPtr - copyFromLeft, copyFromLeft);
      const std::string rightSide(memBuffer, copyFromRight);
      const auto pos = (leftSide + rightSide).find(m_textEnd);
      if (pos != std::string::npos)
      {
        const auto diff = pos - copyFromLeft;
        m_packetDataProcessor = &m_onNewPacket;
        m_onTextPacketDone(&m_buffer->front(), m_currentSize + diff);
        return diff + 4;
      }
    }
    auto memEnd = memBuffer + availableSize;
    auto iter = std::search(memBuffer, memEnd, m_textEnd.begin(), m_textEnd.end());
    if (iter != memEnd)
    {
      std::copy(memBuffer, iter, m_currentPtr);
      const auto diff = iter - memBuffer;
      m_onTextPacketDone(&m_buffer->front(), m_currentSize + diff);
      m_packetDataProcessor = &m_onNewPacket;
      return diff + 4;
    }
    std::copy(memBuffer, memEnd, m_currentPtr);
    m_currentPtr += availableSize;
    m_currentSize += availableSize;
    return availableSize;
  }
  
  std::uint32_t TextAndBinaryPacketReceiver::OnBinaryPacketHeader(const char* memBuffer, const std::uint32_t availableSize)
  {
    const char* dataPtr = memBuffer;
    std::uint32_t dataSize = availableSize;
    
    //casting just to avoid warning.
    if (4 - m_headerSize > static_cast<std::int32_t>(dataSize))
    {
      std::copy(dataPtr, dataPtr + dataSize, m_headerBuffer + m_headerSize);
      m_headerSize += dataSize;
      return dataSize;
    }

    const std::uint32_t headerPartSizeToGet = (4 - m_headerSize);
    std::copy(dataPtr, dataPtr + headerPartSizeToGet , m_headerBuffer + m_headerSize);
    dataPtr += headerPartSizeToGet;
    dataSize -= headerPartSizeToGet;
    m_packetDataProcessor = &m_onBinaryData;
    m_headerSize = 0;

    if (dataSize == 0)
    {
      return headerPartSizeToGet;
    }
    return headerPartSizeToGet + m_onBinaryData(dataPtr, dataSize);
  }
  
  std::uint32_t TextAndBinaryPacketReceiver::OnBinaryPacketData(const char* memBuffer, const std::uint32_t availableSize)
  {
    const std::uint32_t sizeToCopy = (availableSize < m_expectedSize) ? availableSize : m_expectedSize;
    std::copy(memBuffer, memBuffer + sizeToCopy, m_currentPtr);
    m_expectedSize -= sizeToCopy;
    if (m_expectedSize == 0)
    {
      m_packetDataProcessor = &m_onNewPacket;
      m_onBinaryPacketDone(&m_buffer->front(), m_currentPtr - &m_buffer->front() + sizeToCopy);
    }
    else
    {
      m_currentPtr += sizeToCopy;
    }
    return sizeToCopy;
  }
}
