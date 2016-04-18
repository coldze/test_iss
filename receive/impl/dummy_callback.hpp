#ifndef RECEIVE__IMPL__DUMMY_CALLBACK_HPP
#define RECEIVE__IMPL__DUMMY_CALLBACK_HPP

#include "receive/include/icallback.hpp"

namespace Receive
{
  class DummyCallback final : public ICallback
  {
  protected:
    void BinaryPacket(const char*, unsigned int) override {}
    void TextPacket(const char*, unsigned int) override {}
  };
}

#endif
