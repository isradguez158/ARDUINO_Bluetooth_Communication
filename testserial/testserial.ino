#include "Serial_Isra.h"
Serial_Isra Serial_Isra;

unsigned long Delta_T1 = 10000;
unsigned long t_i, t_pr1;
double t;

size_t Arrays_Length = 5;
char Arrays[5] = {0xff, 0xee, 0x05, 0x11, 0x33};

#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  Serial_Isra.INIT();
}

void loop() {
  //Serial_Isra.READ(); 
  
  t_i = micros();
  t = t_i/1000000.0; 
  
  Serial_Isra.READ2(); 
  
  if (t_i - t_pr1 > Delta_T1){
        t_pr1 = t_i;
         Serial_Isra.WRITE(Arrays, Arrays_Length);
           Serial_Isra.Print_serial();
    }


}
