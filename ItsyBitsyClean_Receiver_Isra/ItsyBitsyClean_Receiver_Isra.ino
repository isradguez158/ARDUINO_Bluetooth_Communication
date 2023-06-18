#include <bluefruit.h>
#include "Bluetooth_Isra.h"
BLEClientUart clientUart;
int S1_Length = 20;
int Blue_Length = 32;
char Data_S1[247] = { 0 };
char Data_Blue[247] = { 0 };
char Header[3] = {0xff, 0x88, S1_Length};
uint32_t rx_count = 0;

int DataTest_Length = 5;
char DataTest[5] = {0xff, 0xee, 0xdd, 0x46, 0x55};

int DataTestW_Length = 5;
char DataTestW[5] = {0};
char DataTestW2[5] = {0};
char HeaderT[3] = {0xff, 0xee, 0x05};

unsigned long Delta_T1 = 10000;
unsigned long t_i, t_pr1;
double t;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(10);
  Bluefruit.configCentralBandwidth(BANDWIDTH_MAX);
  Bluefruit.begin(0, 1);
  Bluefruit.setName("Bluefruit52 Central");
  clientUart.begin();
  clientUart.setRxCallback(bleuart_rx_callback);
  Bluefruit.setConnLedInterval(250);
  Bluefruit.Central.setConnectCallback(connect_callback);
  Bluefruit.Central.setDisconnectCallback(disconnect_callback);
  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  Bluefruit.Scanner.useActiveScan(false);
  Bluefruit.Scanner.start(0);
}

void scan_callback(ble_gap_evt_adv_report_t* report)
{
  if ( Bluefruit.Scanner.checkReportForService(report, clientUart) ) {
    Bluefruit.Central.connect(report);
  } else
  {
    Bluefruit.Scanner.resume();
  }
}

void connect_callback(uint16_t conn_handle)
{
  if ( clientUart.discover(conn_handle) )
  {
    clientUart.enableTXD();
  } else
  {
    Bluefruit.disconnect(conn_handle);
  }
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;
}

void bleuart_rx_callback(BLEClientUart& uart_svc)
{
  Blue_Length = uart_svc.available();
  uart_svc.readBytes(Data_Blue, Blue_Length);
  Serial.write(Data_Blue, Blue_Length);
  //Serial1.write(DataTest, DataTest_Length);
  //Print_Test_Read_Blue();
}
void loop()
{
  //Read();
  //Write();
  //Test_Write();

  t_i = micros();
  t = t_i / 1000000.0;

  Test_Read_S1();

  if (t_i - t_pr1 > Delta_T1) {
    t_pr1 = t_i;
    //Serial1.write(Data_Blue, Blue_Length);
    //Serial1.write(DataTest, DataTest_Length);
    //Print_Test_Read_S1();
  }

}

void Test_Write_S1()
{
  Serial1.write(DataTest, DataTest_Length);
}

void Test_Read_S1()
{

  if ( Serial1.available() >= DataTestW_Length )
  {
    DataTestW[0] = Serial1.read(); if (DataTestW[0] == HeaderT[0]) {
      DataTestW[1] = Serial1.read();
      if (DataTestW[1] == HeaderT[1]) {
        DataTestW[2] = Serial1.read();
        //Serial.print("go");
        if (DataTestW[2] == HeaderT[2]) {
          Serial1.readBytes(&DataTestW[3], DataTestW_Length - 3);
          for (int i = 0; i <= DataTestW_Length; i++) {
            DataTestW2[i] = DataTestW[i];
          }
        }
      }
    }
  }
}
void Print_Test_Read_Blue()
{
  Serial.print(Data_Blue[0], HEX);
  Serial.print(" ");
  Serial.print(Data_Blue[1], HEX);
  Serial.print(" ");
  Serial.print(Data_Blue[2], HEX);
  Serial.print(" ");
  Serial.print(Data_Blue[3], HEX);
  Serial.print(" ");
  Serial.print(Data_Blue[4], HEX);
  Serial.println(" ");
}

void Print_Test_Read_S1()
{
  Serial.print(DataTestW2[0], HEX);
  Serial.print(" ");
  Serial.print(DataTestW2[1], HEX);
  Serial.print(" ");
  Serial.print(DataTestW2[2], HEX);
  Serial.print(" ");
  Serial.print(DataTestW2[3], HEX);
  Serial.print(" ");
  Serial.print(DataTestW2[4], HEX);
  Serial.println(" ");
}

void Read()
{
  if ( Serial.available() >= S1_Length )
  {
    Data_S1[0] = Serial.read(); if (Data_S1[0] == Header[0]) {
      Data_S1[1] = Serial.read();
      if (Data_S1[1] == Header[1]) {
        Data_S1[2] = Serial.read();
        if (Data_S1[2] == Header[2]) {
          Serial.readBytes(&Data_S1[3], S1_Length - 3);
        }
      }
    }
  }
}

void Write()
{
  if (Data_S1[2] == Header[2])
  {
    if ( Bluefruit.Central.connected() && clientUart.discovered() )
    {
      clientUart.write(Data_S1, S1_Length);
    }
  }
}

void Test_Write()
{
  if ( Bluefruit.Central.connected() && clientUart.discovered() )
  {
    clientUart.write(DataTest, DataTest_Length);
  }
}
