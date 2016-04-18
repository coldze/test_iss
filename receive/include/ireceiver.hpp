#ifndef RECEIVE__INCLUDE__IRECEIVER_HPP
#define RECEIVE__INCLUDE__IRECEIVER_HPP

#include <memory>

namespace Receive
{
  struct IReceiver
  {
    using SharedPtr = std::shared_ptr<IReceiver>;

    virtual void Receive(const char* data, unsigned int size) = 0;
  };
}

#endif
