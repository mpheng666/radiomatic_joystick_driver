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
  // Joystick configuration here
  Joystick.X(strafing_left_right_analog);
  if(stop_auto_bit || is_estop)
  {
    Joystick.Y(0);
  }
  else
  {
    Joystick.Y(speed_control_analog);
  }

  Joystick.Z(rotate_ccw_cw_analog);
  Joystick.XRotate(backward_forward_analog);
  Joystick.YRotate(ANALOG_RESOLUTION - (water_spray_bit * ANALOG_RESOLUTION));
  Joystick.button(3, action_execution_bit);
  Joystick.dial(action_execution_bit * ANALOG_RESOLUTION);

  if(mode_manual_bit)
  {
    Joystick.Zrotate(0);
  }
  else if(mode_assisted_bit)
  {
    Joystick.Zrotate(ANALOG_RESOLUTION/2);
  }
  else if(mode_auto_bit)
  {
    Joystick.Zrotate(ANALOG_RESOLUTION);
  }

  if(action_startengine_bit)
  {
      Joystick.slider(0);
  }
  else if(action_run_bit)
  {
      Joystick.slider(ANALOG_RESOLUTION/2);
  }
  else if(action_init_bit)
  {
      Joystick.slider(ANALOG_RESOLUTION);
  }

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
    if(ESTOP_BIT)
    {
      is_estop = true;
      break;
    }
    else {
      is_estop = false;
    }
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
  backward_forward_analog = map(can_data.buffer[0], -128, 127, 0, ANALOG_RESOLUTION*SCALING_FACTOR);
  rotate_ccw_cw_analog = map(can_data.buffer[1], -128, 127, ANALOG_RESOLUTION*SCALING_FACTOR, 0);
  strafing_left_right_analog = map(can_data.buffer[3], -128, 127, ANALOG_RESOLUTION*SCALING_FACTOR, 0);
}

void JoyDecoder::decodeAnalog2(const SimpleCANData& can_data) {
  speed_control_analog = map(can_data.buffer[0], 0, 127, 0.2*ANALOG_RESOLUTION, 0.84*ANALOG_RESOLUTION);
}

}