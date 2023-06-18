#include "Serial_Isra.h"

void Serial_Isra::INIT()
{
  SERIAL_Isra.begin(115200);
}

void Serial_Isra::READ()
{
  if (SERIAL_Isra.available())
  {
    ch = SERIAL_Isra.read();
    Packet_Decode(ch);
  }
}

void Serial_Isra::Packet_Decode(uint8_t c)
{
  switch (st)
  {
    case 0: //Read 1st Byte
      if (c == Header[0])
      {
        st = 1;
        Datain[read_count] = c;
        SerialData[read_count] = c;
        read_count += 1;
      }
      break;
    case 1: //Read 2nd Byte
      if (c == Header[1])
      {
        st = 2;
        Datain[read_count] = c;
        SerialData[read_count] = c;
        read_count += 1;
      }
      else
      {
        st = 0;
        read_count = 0;
      }
      break;
    case 2:
      Datain[read_count] = c;
      SerialData[read_count] = c;
      read_count += 1;
      Print_serial();
      if (read_count >= Header[2])
      {
        st = 0;
      }
      break;
    default:
      st = 0;
      break;
  }
}

void Serial_Isra::WRITE(char Msn[], int Msn_Length)
{
  // SERIAL_Isra.write(Msn, 7);
  SERIAL_Isra.write(Msn, Msn_Length);
}


void Serial_Isra::Print_serial()
{
  Serial.print(SerialData2[0], HEX);
  Serial.print(" ");
  Serial.print(SerialData2[1], HEX);
  Serial.print(" ");
  Serial.print(SerialData2[2], HEX);
  Serial.print(" ");
  Serial.print(SerialData2[3], HEX);
  Serial.print(" ");
  Serial.print(SerialData2[4], HEX);
  Serial.print(" ");
  Serial.print(SerialData2[5], HEX);
  Serial.print(" ");
  Serial.print(SerialData2[6], HEX);
  Serial.println(" ");
}

void Serial_Isra::READ2()
{
  if ( SERIAL_Isra.available() >= SerialData_Length )
  {
    SerialData[0] = SERIAL_Isra.read(); if (SerialData[0] == Header[0]) {
      SerialData[1] = SERIAL_Isra.read();
      if (SerialData[1] == Header[1]) {
        SerialData[2] = SERIAL_Isra.read();
        //Serial.print("go");
        if (SerialData[2] == Header[2]) {
          SERIAL_Isra.readBytes(&SerialData[3], SerialData_Length - 3);
          for (int i = 0; i <= SerialData_Length; i++) {
            SerialData2[i] = SerialData[i];
          }
        }
      }
    }
  }
}
