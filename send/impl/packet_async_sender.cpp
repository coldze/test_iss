#include "packet_async_sender.hpp"

#include <iostream>
#include <vector>

namespace
{
  using CanExecute = std::function<bool()>;
  using PacketSender = std::function<void(const char* data, unsigned int size)>;
  
  constexpr double GetMBytesInSecondsCoeff()
  {
    return 1000.0 / (1024 * 1024);
  }

  void ThreadMain(CanExecute canExecute, Receive::IReceiver::SharedPtr dataReceiver, Send::PacketGenerator::SharedPtr dataGenerator, std::function<std::uint32_t()> chunkSizeGenerator, std::uint32_t maxChunkSize)
  {
    try
    {
      std::uint64_t bytesSent = 0;
      auto lastTimestamp = std::chrono::system_clock::now();
      std::vector<char> memBuffer(maxChunkSize, 0);
      for (; canExecute();)
      {
        const auto& currentTimestamp = std::chrono::system_clock::now();
        const auto& periodMs = std::chrono::duration_cast<std::chrono::milliseconds>(currentTimestamp - lastTimestamp).count();
        if (periodMs > 1000)
        {
          lastTimestamp = currentTimestamp;
          const auto currentSpeed = bytesSent * GetMBytesInSecondsCoeff() / periodMs;
          std::cout << "Sent bytes: " << bytesSent << ". Speed: " << currentSpeed << " MBpS." << std::endl;
          bytesSent = 0;
        }
        const std::uint32_t sendingSize = chunkSizeGenerator();
        std::uint32_t availableSize = sendingSize;
        char* sendingPtr = &memBuffer[0];
        for (; availableSize > 0 ;)
        {
          const auto placedPartSize = dataGenerator->PlacePacketTo(sendingPtr, availableSize);
          availableSize -= placedPartSize;
          sendingPtr += placedPartSize;
        }
        
        dataReceiver->Receive(&memBuffer[0], sendingSize);
        bytesSent += sendingSize;
      }
      std::cout << "[INFO] [" << __FUNCTION__ << "] Exiting." << std::endl;
    }
    catch (const std::exception& error)
    {
      std::cout << "[ERROR] [" << __FUNCTION__ << "] Recovering from fatal error. Error: " << error.what() << std::endl;
    }
    catch (...)
    {
      std::cout << "[ERROR] [" << __FUNCTION__ << "] Recovering from fatal error. Error: unknown error." << std::endl;
    }
  }
}

namespace Send
{
  PacketAsyncSender::PacketAsyncSender(Receive::IReceiver::SharedPtr packetReceiver, PacketGenerator::SharedPtr dataGenerator, ChunkSizeGenerator chunkSizeGenerator, std::uint32_t maxChunkSize)
    : m_threadCanRun(true)
  {
    if (packetReceiver.get() == nullptr)
    {
      throw std::runtime_error("Invalid packet-receiver specified.");
    }
    if (dataGenerator.get() == nullptr)
    {
      throw std::runtime_error("Invalid data-generator specified.");
    }
    m_thread = std::make_unique<std::thread>(ThreadMain, [this] () -> bool
      {
        return this->m_threadCanRun;
      }, packetReceiver, dataGenerator, chunkSizeGenerator, maxChunkSize);
  }
      
  PacketAsyncSender::~PacketAsyncSender()
  {
    try
    {
      m_threadCanRun = false;
      m_thread->join();
      std::cout << "[INFO] [" << __FUNCTION__ << "] Thread done." << std::endl;
    }
    catch (const std::exception& error)
    {
      std::cout << "[ERROR] [" << __FUNCTION__ << "] Failed to join thread. Error: " << error.what() << std::endl;
    }
    catch (...)
    {
      std::cout << "[ERROR] [" << __FUNCTION__ << "] Failed to join thread. Error: unknown error." << std::endl;
    }
  }
}
