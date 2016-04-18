#ifndef SEND__INCLUDE__PACKET_FACTORY_HPP
#define SEND__INCLUDE__PACKET_FACTORY_HPP

#include <common/non_copyable.hpp>

#include "packet.hpp"

#include <memory>

namespace Send
{
  class PacketFactory : private Common::NonCopyable
  {
  public:
    using SharedPtr = std::shared_ptr<PacketFactory>;

    virtual std::unique_ptr<Packet> GeneratePacket(std::uint32_t packetSizeToGenerate) const = 0;
  };
}

#endif