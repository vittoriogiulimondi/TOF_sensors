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
#define ROI_SIZE 2
#define MATRIX_SIZE 8

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
    

    
}
*/

// Matrice 8x8 per i centri (deve essere globale)
int centers_matrix[MATRIX_SIZE][MATRIX_SIZE]; 

// Dichiarazione dell'oggetto sensore usando la classe Pololu
VL53L1X sensor; 

void setup(){
    Serial.begin(115200);
    while (!Serial) delay(10);

    Serial.println(F("Starting TOF Configuration:"));

    Wire.begin();
    
    Wire.setClock(400000); // use 400 kHz I2C

  sensor.setTimeout(5000);

  delay(10);

  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }

  // Use long distance mode and allow up to 50000 us (50 ms) for a measurement.
  // You can change these settings to adjust the performance of the sensor, but
  // the minimum timing budget is 20 ms for short distance mode and 33 ms for
  // medium and long distance modes. See the VL53L1X datasheet for more
  // information on range and timing limits.
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(50000);

  // Start continuous readings at a rate of one measurement every 50 ms (the
  // inter-measurement period). This period should be at least as long as the
  // timing budget.
  sensor.startContinuous(60);

    // Imposta la dimensione della ROI usando la funzione Pololu/ST
    // Se ROI_SIZE = 2, stiamo impostando un blocco 2x2 SPAD.
    //sensor.setROISize(ROI_SIZE, ROI_SIZE);

  // Setting ROI size of 4x4 for a 8x8 output matrix

  // calcolo dei centri di ROI


    // code to find the matrix for the centers of the SPAD. Hard to understand but it works!
    

    int b;
    int c;
    int a = 1;
    int spad_index = 1;
    

    for (int i=0; i< MATRIX_SIZE; i++){
        for (int j=0; j < MATRIX_SIZE; j++){
            centers_matrix[i][j] = spad_index;

            a = a + ROI_SIZE;
            b = 0;
            c = 0;
            if (j==3)
            {
                b = 8;
                
            }
            if (j==7){
                b = 8;             
            }
            if (i==3 && j ==7){
                c = 7;
            }
            spad_index = spad_index + (ROI_SIZE) + b + c;
        }

    }
    
    // Ciclo esterno: scorre le RIGHE (indice 'r')
    for (int r = 0; r < 8; r++) {
        
        // Ciclo interno: scorre le COLONNE (indice 'c')
        for (int c = 0; c < 8; c++) {
            
            // Stampa il valore dell'elemento corrente A[r][c]
            Serial.print(centers_matrix[r][c]);
            
            // Aggiunge una tabulazione per spaziatura orizzontale
            Serial.print('\t'); 
        }
        Serial.println();

    

    }
}

void loop(){
    Serial.print(sensor.read());
  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

  Serial.println();
}