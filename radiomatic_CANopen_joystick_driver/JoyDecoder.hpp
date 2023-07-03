#ifndef JOYDECODER_HPP_
#define JOYDECODER_HPP_

#include <Arduino.h>
#include "common_messages.hpp"
#include <unordered_map>

namespace radiomatic_joy {

class JoyDecoder {
  typedef void (JoyDecoder::*decode_function_ptr)(const SimpleCANData& can_data);
public:
  void init();
  void decode(const SimpleCANData& can_data);
  void sendToHost();

private:
  static constexpr uint32_t TX_PDO_DIGITAL1 = 0X181;
  static constexpr uint32_t TX_PDO_DIGITAL2 = 0X281;
  static constexpr uint32_t TX_PDO_ANALOG1 = 0X381;
  static constexpr uint32_t TX_PDO_ANALOG2 = 0X481;
  static constexpr uint32_t ANALOG_RESOLUTION = 1023;
  static constexpr double SCALING_FACTOR = 0.67;

  byte start_transmitter_bit {};
  byte horn_bit {};
  byte mode_manual_bit {};
  byte mode_assisted_bit {};
  byte mode_auto_bit {};
  byte action_startengine_bit {};
  byte action_run_bit {};
  byte action_init_bit {};
  byte a1_bit {};
  byte a2_bit {};
  byte b1_bit {};
  byte b2_bit {};
  byte stop_auto_bit {};
  byte start_auto_bit {};
  byte water_spray_bit {};
  byte action_execution_bit {};
  long rotate_ccw_cw_analog {};
  long backward_forward_analog {};
  long strafing_left_right_analog {};
  long speed_control_analog {};

  const std::unordered_map<uint32_t, decode_function_ptr> DECODE_MAP_{
    { TX_PDO_DIGITAL1, &JoyDecoder::decodeDigital1 },
    { TX_PDO_DIGITAL2, &JoyDecoder::decodeDigital2 },
    { TX_PDO_ANALOG1, &JoyDecoder::decodeAnalog1 },
    { TX_PDO_ANALOG2, &JoyDecoder::decodeAnalog2 }
  };

  void decodeDigital1(const SimpleCANData& can_data);
  void decodeDigital2(const SimpleCANData& can_data);
  void decodeAnalog1(const SimpleCANData& can_data);
  void decodeAnalog2(const SimpleCANData& can_data);
};

}

#endif