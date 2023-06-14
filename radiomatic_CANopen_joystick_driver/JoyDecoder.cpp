#include "JoyDecoder.hpp"

namespace radiomatic_joy {

void JoyDecoder::init() {
  Joystick.useManualSend(true);
}

void JoyDecoder::decode(const SimpleCANData& can_data) {
  if (DECODE_MAP_.count(can_data.id)) {
    (this->*DECODE_MAP_.at(can_data.id))(can_data);
  }
}

void JoyDecoder::sendToHost() {

  // UPDATE THE ANALOG MAPPING HERE
  Joystick.X(rotate_ccw_cw_analog);
  Joystick.Y(backward_forward_analog);
  Joystick.Z(strafing_left_right_analog);
  Joystick.Zrotate(speed_control_analog);

  // UPDATE THE DIGITAL MAPPING HERE
  Joystick.button(1, start_transmitter_bit);
  Joystick.button(2, horn_bit);
  Joystick.button(3, mode_manual_bit);
  Joystick.button(4, mode_assisted_bit);
  Joystick.button(5, mode_auto_bit);
  Joystick.button(6, action_startengine_bit);
  Joystick.button(7, action_run_bit);
  Joystick.button(8, action_init_bit);
  Joystick.button(9, a1_bit);
  Joystick.button(10, a2_bit);
  Joystick.button(11, b1_bit);
  Joystick.button(12, b2_bit);
  Joystick.button(13, stop_auto_bit);
  Joystick.button(14, start_auto_bit);
  Joystick.button(15, water_spray_bit);
  Joystick.button(16, action_execution_bit);

  Joystick.send_now();
}

void JoyDecoder::decodeDigital1(const SimpleCANData& can_data) {
  start_transmitter_bit = (can_data.buffer[0] >> 0) & 1;
  horn_bit = (can_data.buffer[0] >> 1) & 1;
  mode_manual_bit = (can_data.buffer[0] >> 2) & 1;
  mode_assisted_bit = (can_data.buffer[0] >> 3) & 1;
  mode_auto_bit = (can_data.buffer[0] >> 4) & 1;
  action_startengine_bit = (can_data.buffer[0] >> 5) & 1;
  action_run_bit = (can_data.buffer[0] >> 6) & 1;
  action_init_bit = (can_data.buffer[0] >> 7) & 1;

  for (int i = 0; i < 8; ++i) {
    const byte ESTOP_BIT = (can_data.buffer[7] >> i) & 1;
    Joystick.button(i + 17, ESTOP_BIT);
  }
}

void JoyDecoder::decodeDigital2(const SimpleCANData& can_data) {
  a1_bit = (can_data.buffer[0] >> 0) & 1;
  a2_bit = (can_data.buffer[0] >> 1) & 1;
  b1_bit = (can_data.buffer[0] >> 2) & 1;
  b2_bit = (can_data.buffer[0] >> 3) & 1;
  stop_auto_bit = (can_data.buffer[0] >> 4) & 1;
  start_auto_bit = (can_data.buffer[0] >> 5) & 1;
  water_spray_bit = (can_data.buffer[0] >> 6) & 1;
  action_execution_bit = (can_data.buffer[0] >> 7) & 1;

  for (int i = 0; i < 8; ++i) {
    const byte ESTOP_INVERTED_BIT = (can_data.buffer[7] >> i) & 1;
    Joystick.button(i + 25, ESTOP_INVERTED_BIT);
  }
}

void JoyDecoder::decodeAnalog1(const SimpleCANData& can_data) {
  backward_forward_analog = map(can_data.buffer[0], -128, 127, 1023, 0);
  rotate_ccw_cw_analog = map(can_data.buffer[1], -128, 127, 0, 1023);
  strafing_left_right_analog = map(can_data.buffer[3], -128, 127, 0, 1023);
}

void JoyDecoder::decodeAnalog2(const SimpleCANData& can_data) {
  speed_control_analog = map(can_data.buffer[0], 0, 127, 0, 1023);
}

}