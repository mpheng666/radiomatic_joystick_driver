#include "CANReader.hpp"
#include "JoyDecoder.hpp"

using namespace radiomatic_joy;

CANReader can_reader(125000);
JoyDecoder joy_decoder;

void setup(void) {
  can_reader.init();
  joy_decoder.init();
}

void loop() {
  const auto data = can_reader.read();
  if (data.id != 0X999) {
    joy_decoder.decode(data);
  }
  joy_decoder.sendToHost();
}
