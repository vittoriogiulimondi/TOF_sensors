/* This is an old PicoLowLevel test code, used only to verify that the Makefile works correctly.
Do not upload it to the Pico — it may contain errors or modified sections.
Use it exclusively for Makefile testing. */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "mcp2515.h"
#include "CanWrapper.h"

#include "include/definitions.h"
#include "include/mod_config.h"
#include "include/communication.h"

void setup(){
Serial.begin(115200);

}
void loop(){
Serial.println("ciao");
delay(1000);

}