#ifndef RECEIVE__INCLUDE__PACKET_BUILDING_RECEIVER_HPP
#define RECEIVE__INCLUDE__PACKET_BUILDING_RECEIVER_HPP

#include <common/non_copyable.hpp>

#include <memory>

namespace Receive
{
  class PacketBuildingReceiver : private Common::NonCopyable
  {
  public:
    using UniquePtr = std::unique_ptr<PacketBuildingReceiver>;
    
    virtual std::uint32_t OnPacketData(const char* memBuffer, const std::uint32_t availableSize) = 0;
  };
}

#endif
