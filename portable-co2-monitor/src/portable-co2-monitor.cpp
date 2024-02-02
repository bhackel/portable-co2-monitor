// Portable CO2 monitor with Senseair S8

#include <Arduino.h>
#include "s8_uart.h"

#include "portable-co2-monitor.h"


S8_UART *sensorS8;
S8_sensor sensor;

int maxCo2 = 0;
double avgCo2 = 0.0;
unsigned long numMeasurements = 0;
int lastMeasurementTime = 0;
int curTime = 0;

// Set up BLE services and characteristics
BLEService co2Service("FA11B9C1-426C-485B-8184-03EC0E0741EC");
BLEIntCharacteristic co2CurLevelCharacteristic("FA116969-426C-485B-8184-03EC0E0741EC", BLERead | BLENotify);
BLEIntCharacteristic co2MaxLevelCharacteristic("FA116970-426C-485B-8184-03EC0E0741EC", BLERead | BLENotify);
BLEIntCharacteristic co2AvgLevelCharacteristic("FA116971-426C-485B-8184-03EC0E0741EC", BLERead | BLENotify);
BLEService ledService("8370846E-2189-44BC-86F9-76FD3772BA84");
BLEIntCharacteristic ledStatusCharacteristic("83706969-2189-44BC-86F9-76FD3772BA84", BLEWrite);

void setup() {
  // Set up Serial for logging
  Serial.begin(9600);
  Serial.println("Starting up...");

  // Set up BLE services and characteristics
  while (!BLE.begin()) {
    Serial.println("Waiting for BLE to start");
    delay(1);
  }
  BLE.setLocalName("CO2Monitor");
  BLE.setAdvertisedService(co2Service); // sets the primary service
  BLE.addService(co2Service);
  co2Service.addCharacteristic(co2CurLevelCharacteristic);
  co2Service.addCharacteristic(co2MaxLevelCharacteristic);
  co2Service.addCharacteristic(co2AvgLevelCharacteristic);
  co2CurLevelCharacteristic.setEventHandler(BLERead, co2CurLevelCharacteristicRead);
  co2MaxLevelCharacteristic.setEventHandler(BLERead, co2MaxLevelCharacteristicRead);
  co2AvgLevelCharacteristic.setEventHandler(BLERead, co2AvgLevelCharacteristicRead);
  BLE.advertise();

  // Initialize UART connection and S8 sensor
  Serial1.begin(S8_BAUDRATE);
  sensorS8 = new S8_UART(Serial1); 

  // Check if S8 sensor is available by reading firmware version
  sensorS8->get_firmware_version(sensor.firm_version);
  int firmwareStrLen = strlen(sensor.firm_version);
  int numTries = 1;
  int s8AttemptsLimit = 120;
  while (firmwareStrLen == 0 && numTries++ < s8AttemptsLimit) {
      delay(250);
      sensorS8->get_firmware_version(sensor.firm_version);
      firmwareStrLen = strlen(sensor.firm_version);
  }

  // Log basic S8 sensor info
  Serial.println("SenseAir S8 Firmware:");
  if (firmwareStrLen == 0) Serial.print("N/A");
  else Serial.println(sensor.firm_version);
  sensor.sensor_id = sensorS8->get_sensor_ID();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // Check if a device (central) connects to this device (peripheral)
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to ");
    Serial.println(central.address());
    while (central.connected()) {
      // Check if enough time has passed since last check
      curTime = millis();
      if (curTime <= (lastMeasurementTime + MEASUREMENT_INTERVAL)) {
        continue;
      }
      lastMeasurementTime = millis();

      
      char printBuffer[17];
      // Get current CO2 measurement
      sensor.co2 = sensorS8->get_co2();
      sprintf(printBuffer, "%4d", sensor.co2);
      Serial.println("CO2: " + String(printBuffer));
      // Caulcualte max since start-up
      if (sensor.co2 > maxCo2) { 
        maxCo2 = sensor.co2; 
      }
      sprintf(printBuffer, "%-4d", maxCo2);
      Serial.println("Max: " + String(printBuffer));
      // Calculate average CO2 level
      if(numMeasurements == 0) {
          avgCo2 = (double) sensor.co2;
      } else {
          avgCo2 = (avgCo2 * (double)numMeasurements + (double)sensor.co2) / (double)(numMeasurements + 1);
      }
      numMeasurements++;
      int avgCo2Int = round(avgCo2);
      sprintf(printBuffer, "%4d", avgCo2Int);
      Serial.println("Ave: " + String(printBuffer));

      // Update LED state
      if (ledStatusCharacteristic.written()) {
        Serial.println("LED status changed");
        int status = boolean(ledStatusCharacteristic.value()) ? HIGH : LOW;
        digitalWrite(LED_BUILTIN, status);    
      }

      // Update BLE characteristics
      co2CurLevelCharacteristic.writeValue(sensor.co2);
      co2MaxLevelCharacteristic.writeValue(maxCo2);
      co2AvgLevelCharacteristic.writeValue(avgCo2Int);
    }
  }
}

/* Read request handler for characteristics */
void co2CurLevelCharacteristicRead(BLEDevice central, BLECharacteristic characteristic) {
  co2CurLevelCharacteristic.writeValue(sensor.co2);
}
void co2MaxLevelCharacteristicRead(BLEDevice central, BLECharacteristic characteristic) {
  co2MaxLevelCharacteristic.writeValue(maxCo2);
}
void co2AvgLevelCharacteristicRead(BLEDevice central, BLECharacteristic characteristic) {
  co2AvgLevelCharacteristic.writeValue(round(avgCo2));
}