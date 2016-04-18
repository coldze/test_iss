#ifndef SEND__IMPL__PACKET_GENERATOR_HPP
#define SEND__IMPL__PACKET_GENERATOR_HPP

#include <common/non_copyable.hpp>

#include "send/include/packet_factory.hpp"

#include <functional>
#include <memory>
#include <vector>

namespace Send
{
  class PacketGenerator final : private Common::NonCopyable
  {
  public:
    using PacketSizeGenerator = std::function<std::uint32_t ()>;
    using SharedPtr = std::shared_ptr<PacketGenerator>;

    PacketGenerator(std::vector<PacketFactory::SharedPtr>&& packetFactories, PacketSizeGenerator&& packetSizeGenerator);
    
    std::uint32_t PlacePacketTo(char* memBuffer, const std::uint32_t availableSize);

  private:
    const std::vector<PacketFactory::SharedPtr> m_packetFactories;
    const PacketSizeGenerator m_packetSizeGenerator;
    Send::Packet::UniquePtr m_packet;
    
    void Regenerate();
  };
}

#endif