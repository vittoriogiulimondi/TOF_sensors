/* This is an old PicoLowLevel test code, used only to verify that the Makefile works correctly.
Do not upload it to the Pico — it may contain errors or modified sections.
Use it exclusively for Makefile testing. */

#include <Arduino.h>
#include <Wire.h>
/* #include <SPI.h>
#include "mcp2515.h"
#include "CanWrapper.h"

#include "include/definitions.h"
#include "include/mod_config.h"
#include "include/communication.h"
*/
//#include "Adafruit_VL53L1X.h"
#include "VL53L1X.h"

#define MATRIX_SIZE 4

#define SDA_PIN_ZERO 2
#define SCL_PIN_ZERO 3

//int received_matrix[MATRIX_SIZE];

int received_matrix[MATRIX_SIZE];

//const size_t BYTES_FROM_ZERO = sizeof(received_matrix);

const size_t BYTES_FROM_ZERO = sizeof(received_matrix);

const int ZERO_ADDRESS = 0x42;
const long BAUD_RATE = 100000;

//TwoWire pico2zero_Bus(i2c1, SDA_PIN_ZERO, SCL_PIN_ZERO);

void setup(){
  while (!Serial) {}
  Serial.begin(115200);
 
  //pinMode(SDA_PIN_ZERO, INPUT_PULLUP);
  //pinMode(SCL_PIN_ZERO, INPUT_PULLUP);

  Wire1.setSDA(2);  // GP4
  Wire1.setSCL(3);  // GP5
  Wire1.begin();
  Wire1.setClock(BAUD_RATE);
    while(!Serial) delay(10);
  Serial.println("Pico Master Initialized");
  delay(1000);
}

void loop(){
  Wire1.requestFrom(ZERO_ADDRESS, BYTES_FROM_ZERO);
  
  if (Wire1.available() ){
    int tof_array[MATRIX_SIZE];
    byte temp_buffer[BYTES_FROM_ZERO];

    for (size_t i=0; i<BYTES_FROM_ZERO; i++) {
      temp_buffer[i] = Wire1.read();
    }

    memcpy(received_matrix, temp_buffer, BYTES_FROM_ZERO);

    Serial.println("\nMaster: Array received successfully!");
    
    for (size_t i = 0; i < MATRIX_SIZE; i++) {
        Serial.print("Elemento [");
        Serial.print(i);
        Serial.print("]: ");
        Serial.println(received_matrix[i]);
    } 
  }
  else {
    Serial.println("Master: Incomplete data received.");
  }

  delay(3000);
}