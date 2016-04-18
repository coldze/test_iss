#ifndef RECEIVE__IMPL__TEXT_AND_BINARY_PACKET_RECEIVER_HPP
#define RECEIVE__IMPL__TEXT_AND_BINARY_PACKET_RECEIVER_HPP

#include "receive/include/packet_building_receiver.hpp"

#include <functional>
#include <string>
#include <vector>

namespace Receive
{
  class TextAndBinaryPacketReceiver final : public Receive::PacketBuildingReceiver
  {
  public:
  	using ReceivedPacketCallback = std::function<void(const char* data, unsigned int size)>;

    TextAndBinaryPacketReceiver(std::uint32_t bufferSize, char binaryHeader, const std::string& textEnding, ReceivedPacketCallback binaryCallback, ReceivedPacketCallback textCallback);
    
  protected:
    std::uint32_t OnPacketData(const char* memBuffer, const std::uint32_t availableSize) override;
    
  private:
    using DataProcessor = std::function<std::uint32_t(const char* memBuffer, const std::uint32_t availableSize)>;
    
    //Those lambdas are just to get rid of IF-s with checking internal receiving state: are we expecting new packet or text/binary data, etc.
    DataProcessor* m_packetDataProcessor;
    DataProcessor m_onNewPacket;
    DataProcessor m_onBinaryHeader;
    DataProcessor m_onBinaryData;
    DataProcessor m_onTextData;
    
    const char m_binaryHeader;
    const std::string m_textEnd;
    
    const ReceivedPacketCallback m_onBinaryPacketDone;
    const ReceivedPacketCallback m_onTextPacketDone;
    
    std::uint32_t m_expectedSize;
    char* m_headerBuffer;
    char m_headerSize;
    std::unique_ptr<std::vector<char>> m_buffer;
    char* m_currentPtr;
    std::uint32_t m_currentSize;
    
    std::uint32_t OnNewPacket(const char* memBuffer, const std::uint32_t availableSize);
    std::uint32_t OnTextPacketData(const char* memBuffer, const std::uint32_t availableSize);
    std::uint32_t OnBinaryPacketHeader(const char* memBuffer, const std::uint32_t availableSize);
    std::uint32_t OnBinaryPacketData(const char* memBuffer, const std::uint32_t availableSize);
  };
}

#endif
