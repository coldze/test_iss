#ifndef SEND__IMPL__PACKET_ASYNC_SENDER_HPP
#define SEND__IMPL__PACKET_ASYNC_SENDER_HPP

#include "receive/include/ireceiver.hpp"

#include "send/impl/packet_generator.hpp"

#include <atomic>
#include <functional>
#include <memory>
#include <thread>

namespace Send
{
  class PacketAsyncSender final : private Common::NonCopyable
  {
  public:
    using ChunkSizeGenerator = std::function<std::uint32_t()>;
    
    PacketAsyncSender(Receive::IReceiver::SharedPtr packetReceiver, PacketGenerator::SharedPtr dataGenerator, ChunkSizeGenerator chunkSizeGenerator, std::uint32_t maxChunkSize);
    virtual ~PacketAsyncSender();

  private:
    std::atomic<bool> m_threadCanRun;
    std::unique_ptr<std::thread> m_thread;
  };
}

#endif