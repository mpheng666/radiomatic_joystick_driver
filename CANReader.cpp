#include "CANReader.hpp"
#include <Arduino.h>

namespace radiomatic_joy {

CANReader::CANReader(uint32_t baudrate) {
  can1.begin();
  can1.setBaudRate(baudrate);
  init();
}

void CANReader::init() {
  CAN_message_t INIT_MSG_;
  CAN_message_t trigger_msg;
  trigger_msg.id = 0x00;
  trigger_msg.len = 2;
  trigger_msg.buf[0] = 1;
  trigger_msg.buf[1] = 0;
  can1.write(trigger_msg);
}

SimpleCANData CANReader::read() {

  SimpleCANData ret_val;

  CAN_message_t read_buffer;
  if (can1.read(read_buffer)) {
    ret_val.id = read_buffer.id;
    for (uint8_t i = 0; i < 8; i++) {
      ret_val.buffer[i] = read_buffer.buf[i];
    }
    // ret_val.print();
    last_connected_ts_ = millis();
  }

  if (ret_val.id == 0x701) {
    if (!isConnected(ret_val)) {
      reConnect();
    }
  }

  if(millis() - last_connected_ts_ > TIMEOUT_MS_)
  {
    reConnect();
    last_connected_ts_ = millis();
  }

  return ret_val;
}

bool CANReader::isConnected(const SimpleCANData& can_data) {
  return (can_data.buffer[0] != 127);
}

void CANReader::reConnect() {
  Serial.println("INITIALIZING");
  init();
}

}