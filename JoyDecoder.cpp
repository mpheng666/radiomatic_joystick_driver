#include "JoyDecoder.hpp"

namespace radiomatic_joy {

void JoyDecoder::init() {
  Joystick.useManualSend(true);
}

void JoyDecoder::decode(const SimpleCANData& can_data) {
  if (DECODE_MAP_.count(can_data.id)) {
    (this->*DECODE_MAP_[can_data.id])(can_data);
  }
}

void JoyDecoder::sendToHost() {
  Joystick.send_now();
}

void JoyDecoder::decodeDigital1(const SimpleCANData& can_data) {
  for (int i = 0; i < 8; ++i) {
    byte bit = (can_data.buffer[0] >> i) & 1;
    Joystick.button(i + 1, bit);
  }

  for (int i = 0; i < 8; ++i) {
    byte bit = (can_data.buffer[7] >> i) & 1;
    Joystick.button(i + 17, bit);
  }
}

void JoyDecoder::decodeDigital2(const SimpleCANData& can_data) {
    for (int i = 0; i < 8; ++i) {
    byte bit = (can_data.buffer[0] >> i) & 1;
    Joystick.button(i + 9, bit);
  }

  for (int i = 0; i < 8; ++i) {
    byte bit = (can_data.buffer[7] >> i) & 1;
    Joystick.button(i + 25, bit);
  }
}

void JoyDecoder::decodeAnalog1(const SimpleCANData& can_data) {
  auto a_x = map(can_data.buffer[1], -128, 127, 0, 1023);
  Joystick.X(a_x);
  auto a_y = map(can_data.buffer[0], -128, 127, 1023, 0);
  Joystick.Y(a_y);
  auto a_z = map(can_data.buffer[3], -128, 127, 0, 1023);
  Joystick.Z(a_z);
}

void JoyDecoder::decodeAnalog2(const SimpleCANData& can_data) {
  auto a_z_speed_control = map(can_data.buffer[0], 0, 127, 0, 1023);
  Joystick.Zrotate(a_z_speed_control);
}

}