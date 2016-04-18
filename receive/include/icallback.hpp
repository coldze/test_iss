#ifndef RECEIVE__INCLUDE__ICALLBACK_HPP
#define RECEIVE__INCLUDE__ICALLBACK_HPP

#include <memory>

namespace Receive
{
  struct ICallback
  {
    using SharedPtr = std::shared_ptr<ICallback>;

    virtual void BinaryPacket(const char* data, unsigned int size) = 0;
    virtual void TextPacket(const char* data, unsigned int size) = 0;
  };
}

#endif
