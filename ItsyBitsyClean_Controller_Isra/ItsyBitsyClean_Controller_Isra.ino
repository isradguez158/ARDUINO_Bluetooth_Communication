#include <bluefruit.h>
#include "Bluetooth_Isra.h"

#define PACKET_NUM    1000
int S1_Length = 32; //Max message lenght
int Blue_Length = 20;
char Data_S1[247] = {0};
char Data_Blue[247] = {0};
int Header[3] = {0xff, 0x88, S1_Length};
BLEDis bledis;
BLEUart bleuart;

int DataTest_Length = 5;
char DataTest[5] = {0xff, 0xee, 0xdd, 0x46, 0x55};

int DataTestW_Length = 5;
char DataTestW[5] = {0};
char DataTestW2[5] = {0};
char HeaderT[3] = {0xff, 0xee, 0x05};

unsigned long Delta_T1 = 10000;
unsigned long t_i, t_pr1;
double t;

uint32_t rxCount = 0;
uint32_t rxStartTime = 0;
uint32_t rxLastTime = 0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(10);
  Serial.println("Bluefruit52 Throughput Example");
  Serial.println("------------------------------\n");

  Setup_Bluetooth();
  startAdv();
}

void Setup_Bluetooth() {
  Bluefruit.autoConnLed(true);
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  Bluefruit.begin();
  Bluefruit.setTxPower(4);
  Bluefruit.setName("Bluefruit52");
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);
  Bluefruit.Periph.setConnInterval(6, 12); // 7.5 - 15 ms
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();
  bleuart.begin();
  bleuart.setRxCallback(bleuart_rx_callback);
  bleuart.setNotifyCallback(bleuart_notify_callback);
}

void startAdv(void)
{
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addService(bleuart);
  Bluefruit.ScanResponse.addName();
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);
  Bluefruit.Advertising.setFastTimeout(30);
  Bluefruit.Advertising.start(0);
}

void connect_callback(uint16_t conn_handle)
{
  BLEConnection* conn = Bluefruit.Connection(conn_handle);
  Serial.println("Connected");
  Serial.println("Request to change PHY");
  conn->requestPHY();
  Serial.println("Request to change Data Length");
  conn->requestDataLengthUpdate();
  Serial.println("Request to change MTU");
  conn->requestMtuExchange(247);
  delay(1000);
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;
  Serial.println();
  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
}

void bleuart_rx_callback(uint16_t conn_hdl)
{
  (void) conn_hdl;
  Blue_Length = bleuart.available();
  bleuart.readBytes(Data_Blue, Blue_Length);
}

void bleuart_notify_callback(uint16_t conn_hdl, bool enabled)
{
  if ( enabled )
  {
    Serial.println("notify_callback");
  }
}

void loop() {
//  Read();
//  Write();
//  Test_Write();

  t_i = micros();
  t = t_i / 1000000.0;

  Test_Read_S1();

  if (t_i - t_pr1 > Delta_T1) {
    t_pr1 = t_i;
    Serial1.write(Data_Blue, Blue_Length);
    //Serial1.write(DataTest, DataTest_Length);
    //Test_Write_S1();
    Test_Write();
    Print_Test_Read_S1();
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
  if ( Serial1.available() >= S1_Length )
  {
    Data_S1[0] = Serial1.read(); if (Data_S1[0] == Header[0]) {
      Data_S1[1] = Serial1.read();
      if (Data_S1[1] == Header[1]) {
        Data_S1[2] = Serial1.read();
        if (Data_S1[2] == Header[2]) {
          Serial1.readBytes(&Data_S1[3], S1_Length - 3);
        }
      }
    }
  }
}

void Write()
{
  if (Data_S1[2] == Header[2])
  {
    if (Bluefruit.connected() && bleuart.notifyEnabled())
    {
      bleuart.write(Data_S1, S1_Length);
    }
  }
}

void Test_Write()
{
  if (Bluefruit.connected() && bleuart.notifyEnabled())
  {
    bleuart.write(DataTest, DataTest_Length);
  }
}
