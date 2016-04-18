#ifndef RECEIVE__IMPL__CYCLIC_RECEIVER_HPP
#define RECEIVE__IMPL__CYCLIC_RECEIVER_HPP

#include "receive/include/ireceiver.hpp"
#include "receive/include/packet_building_receiver.hpp"

namespace Receive
{
  class CyclicReceiver final : public IReceiver
  {
  public:
    explicit CyclicReceiver(PacketBuildingReceiver::UniquePtr packetBuilder);
    
  protected:
    void Receive(const char* data, unsigned int size) override;
    
  private:
    const PacketBuildingReceiver::UniquePtr m_packetBuildingReceiver;
  };
}

#endif
