#ifndef COMMON_MESSAGES_HPP_
#define COMMON_MESSAGES_HPP_

namespace radiomatic_joy {

struct SimpleCANData {
  uint32_t id = 0x999;
  int8_t buffer[8];

  void print() {
    if (id != 0x999) {
      Serial.print("CAN ID: ");
      Serial.println(id, HEX);
      Serial.print("BUFFER: ");
      for (int i = 0; i < 8; ++i) {
        Serial.print(" ");
        Serial.print(buffer[i]);
      }
      Serial.println("");
    }
  }
};

}
#endif