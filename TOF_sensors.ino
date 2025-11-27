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

/*
#define IRQ_PIN 2
#define XSHUT_PIN 3



Adafruit_VL53L1X vl53 = Adafruit_VL53L1X(XSHUT_PIN, IRQ_PIN);
*/
#define ROI_SIZE 4
#define MATRIX_SIZE 4

// For the definition of the center of ROI we find the SPAD configuration on both the pololu library and the ST user manual.
// Having defined a ROI size of 4, the center falls in between the 4 SPADs. The manual tells to choose as a center the upper-right position!
/*
void calculate_ROI_centers(){
    const int8_t ROI_SIZE = 2;
    const int8_t MATRIX_SIZE = 16/ROI_SIZE;

    int8_t centers_matrix[MATRIX_SIZE][MATRIX_SIZE]

    int a = (ROI_SIZE-1) / 2;
    int spad_index = a + 8*(a)

    for (i=1; i<= MATRIX_SIZE; i++){
        for (j=1; j < MATRIX_SIZE, j++){
            centers_matrix[i][j] = spad_index;

            a = a + ROI_SIZE
            spad_index = spad_index + 8*(ROI_SIZE)
        }
    }
    
/* Table of SPAD locations. Each SPAD has a number which is not obvious.
* Pin 1 *
* 128,136,144,152,160,168,176,184, 192,200,208,216,224,232,240,248
* 129,137,145,153,161,169,177,185, 193,201,209,217,225,233,241,249
* 130,138,146,154,162,170,178,186, 194,202,210,218,226,234,242,250
* 131,139,147,155,163,171,179,187, 195,203,211,219,227,235,243,251
* 132,140,148,156,164,172,180,188, 196,204,212,220,228,236,244,252
* 133,141,149,157,165,173,181,189, 197,205,213,221,229,237,245,253
* 134,142,150,158,166,174,182,190, 198,206,214,222,230,238,246,254
* 135,143,151,159,167,175,183,191, 199,207,215,223,231,239,247,255
* 127,119,111,103, 95, 87, 79, 71, 63, 55, 47, 39, 31, 23, 15, 7
* 126,118,110,102, 94, 86, 78, 70, 62, 54, 46, 38, 30, 22, 14, 6
* 125,117,109,101, 93, 85, 77, 69, 61, 53, 45, 37, 29, 21, 13, 5
* 124,116,108,100, 92, 84, 76, 68, 60, 52, 44, 36, 28, 20, 12, 4
* 123,115,107, 99, 91, 83, 75, 67, 59, 51, 43, 35, 27, 19, 11, 3
* 122,114,106, 98, 90, 82, 74, 66, 58, 50, 42, 34, 26, 18, 10, 2
* 121,113,105, 97, 89, 81, 73, 65, 57, 49, 41, 33, 25, 17, 9, 1
* 120,112,104, 96, 88, 80, 72, 64, 56, 48, 40, 32, 24, 16, 8, 0
    
}

// Matrice 4x4
int centers_matrix[MATRIX_SIZE][MATRIX_SIZE] = {
        {145, 177, 209, 241}, // Riga 0
        {149, 181, 213, 245},    // Riga 1
        {110, 78, 46, 14},    // Riga 2
        {106, 74, 42, 10}     // Riga 3
};

*/
// Matrice 4x1
int centers_matrix[MATRIX_SIZE] = {193, 197, 62, 58};

// Matrice 8x8 per i centri (deve essere globale)
//int centers_matrix[MATRIX_SIZE][MATRIX_SIZE]; 

// The number of sensors in your system.
const uint8_t sensorCount = 2;

// The Arduino pin connected to the XSHUT pin of each sensor.
const uint8_t xshutPins[sensorCount] = { 0, 1 };

VL53L1X sensors[sensorCount];

void setup(){
    while (!Serial) {}
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C

  // Disable/reset all sensors by driving their XSHUT pins low.
  for (uint8_t i = 0; i < sensorCount; i++)
  {
    pinMode(xshutPins[i], OUTPUT);
    digitalWrite(xshutPins[i], LOW);
  }

  // Enable, initialize, and start each sensor, one by one.
  for (uint8_t i = 0; i < sensorCount; i++)
  {
    // Stop driving this sensor's XSHUT low. This should allow the carrier
    // board to pull it high. (We do NOT want to drive XSHUT high since it is
    // not level shifted.) Then wait a bit for the sensor to start up.
    pinMode(xshutPins[i], INPUT);
    delay(10);

    sensors[i].setTimeout(500);
    if (!sensors[i].init())
    {
      Serial.print("Failed to detect and initialize sensor ");
      Serial.println(i);
      while (1);
    }

    // Each sensor must have its address changed to a unique value other than
    // the default of 0x29 (except for the last one, which could be left at
    // the default). To make it simple, we'll just count up from 0x2A.
    sensors[i].setAddress(0x2A + i);

    sensors[i].startContinuous(50);

    sensors[i].setROISize(ROI_SIZE,ROI_SIZE);
  }
    /*
    // Ciclo esterno: scorre le RIGHE (indice 'r')
    for (int r = 0; r < MATRIX_SIZE; r++) {
        
        // Ciclo interno: scorre le COLONNE (indice 'c')
        for (int c = 0; c < MATRIX_SIZE; c++) {
            
            // Stampa il valore dell'elemento corrente A[r][c]
            Serial.print(centers_matrix[r][c]);
            
            // Aggiunge una tabulazione per spaziatura orizzontale
            Serial.print('\t'); 
        }
        Serial.println();
    }
    */

    
}

void loop(){
   // Serial.print(sensor.read());
   for (int i=0; i<sensorCount; i++){
        Serial.print("Distance Matrix: ");
        Serial.println(i);
        if (sensors[i].timeoutOccurred()) { Serial.print(" TIMEOUT"); }
        for (int r = 0; r < MATRIX_SIZE; r++) {
                //for (int c = 0; c < MATRIX_SIZE; c++) {
                    
                    // 1. Imposta il centro della ROI
                    int center_spad = centers_matrix[r];
                    sensors[i].setROICenter(center_spad);

                    delay(5);
                    
                    // 2. Legge la distanza (attende automaticamente il risultato)
                    int16_t distance_mm = sensors[i].read(); 
                    
                    // 3. Stampa il risultato in formato tabellare (o lo salva)
                    Serial.print(distance_mm);
                    
                    // Aggiunge una tabulazione per spaziatura orizzontale
                    Serial.print('\t'); 
                    
                    // Opzionale: Aggiungi un piccolo ritardo per non sovraccaricare il bus/serial monitor
                    delay(1); 
                }
                // Nuova riga alla fine di ogni riga della matrice
                Serial.println();
            }
            
            // Ritardo tra una scansione completa (64 ROI) e la successiva
            // Poiché ogni lettura richiede ~50ms (Timing Budget), l'intera scansione 8x8
            // impiega circa 64 * 50ms = 3.2 secondi.
            delay(10);

        Serial.println();
//}
  
}