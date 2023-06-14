#ifndef CANREARDER_HPP_
#define CANREARDER_HPP_

#include <FlexCAN_T4.h>
#include "common_messages.hpp"

namespace radiomatic_joy
{
  class CANReader
  {
    public:
      CANReader(uint32_t baudrate);
      void init();
      SimpleCANData read();

    private:
      static constexpr long TIMEOUT_MS_ = 2000;
      FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;
      unsigned long last_connected_ts_ {millis()};

      bool isConnected(const SimpleCANData& can_data);
      void reConnect();
  };
}
#endif