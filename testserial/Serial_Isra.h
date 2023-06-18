#include <Arduino.h>

#define SERIAL_Isra (Serial1)

class Serial_Isra
{
  public:
    void INIT();
    void READ();
    void READ2();
    void WRITE(char Msn[], int Msn_Length);
    void Packet_Decode(uint8_t c);
    void Print_serial();
    char Header[3] = {0xff, 0xee, 0xdd};
    uint8_t SerialData[5];
    uint8_t SerialData2[5];
    int SerialData_Length=5;
    uint8_t ch;
    int count1 = 0;
    uint8_t st = 0;
    uint8_t Datain[5];
    int read_count = 0;

  private:

};
