#include "receive/impl/cyclic_receiver.hpp"
#include "receive/impl/dummy_callback.hpp"
#include "receive/impl/text_and_binary_packet_receiver.hpp"
#include "send/impl/binary_packet_factory.hpp"
#include "send/impl/packet_async_sender.hpp"
#include "send/impl/packet_generator.hpp"
#include "send/impl/text_packet_factory.hpp"

#include <memory>
#include <string>
#include <vector>

namespace
{
  const std::string TEXT_PACKET_ENDING("\r\n\r\n");
  const char BINARY_PACKET_HEADER = 0x24;
  const std::uint32_t PACKET_SIZE_MAX = 4096;
  const std::uint32_t PACKET_SIZE_MIN = 32;
  const std::uint32_t CHUNK_SIZE_MAX = 1024;
  const std::uint32_t CHUNK_SIZE_MIN = 128;
  
  std::uint32_t GetPacketBufferMaxSize()
  {
    return PACKET_SIZE_MAX + (5 < TEXT_PACKET_ENDING.size() ? TEXT_PACKET_ENDING.size() : 5);
  }
  
  constexpr std::uint32_t GetPacketSizeDivider()
  {
    return PACKET_SIZE_MAX - PACKET_SIZE_MIN;
  }
  
  constexpr std::uint32_t GetChunkSizeDivider()
  {
    return CHUNK_SIZE_MAX - CHUNK_SIZE_MIN;
  }
  
  constexpr std::uint32_t GetReceiverBufferSize()
  {
    return PACKET_SIZE_MAX * 4;
  }
}

int main(int, char*[])
{
  std::srand(static_cast<unsigned int>(std::time(0)));
  {
    std::vector<Send::PacketFactory::SharedPtr> packetFactories;
    //Using shared buffer with allocation on heap, just to skip allocation at each iteration and save some memory...
    std::shared_ptr<std::vector<char>> packetBuffer = std::make_shared<std::vector<char>>(GetPacketBufferMaxSize(), 0);
    packetFactories.emplace_back(std::make_unique<Send::BinaryPacketFactory>(packetBuffer, BINARY_PACKET_HEADER));
    packetFactories.emplace_back(std::make_unique<Send::TextPacketFactory>(packetBuffer, TEXT_PACKET_ENDING));
    auto dataGenerator = std::make_shared<Send::PacketGenerator>(std::move(packetFactories), []() -> std::uint32_t
      {
        return std::rand() % GetPacketSizeDivider() + PACKET_SIZE_MIN;
      });
    const Receive::ICallback::SharedPtr callbackObject = std::make_shared<Receive::DummyCallback>();
    auto binaryPacketCallback = [callbackObject](const char* data, unsigned int size)
      {
        callbackObject->BinaryPacket(data, size);
      };
    auto textPacketCallback = [callbackObject](const char* data, unsigned int size)
      {
        callbackObject->TextPacket(data, size);
      };
    auto receiver = std::make_shared<Receive::CyclicReceiver>(std::make_unique<Receive::TextAndBinaryPacketReceiver>(GetReceiverBufferSize(), BINARY_PACKET_HEADER, TEXT_PACKET_ENDING, binaryPacketCallback, textPacketCallback));
    Send::PacketAsyncSender holder(receiver, dataGenerator, []() -> std::uint32_t
      {
      return std::rand() % GetChunkSizeDivider() + CHUNK_SIZE_MIN;
      }, CHUNK_SIZE_MAX);
    getchar();
  }
  return 0;
}
