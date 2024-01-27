// Portable CO2 monitor with Senseair S8

#include <Arduino.h>
#include "s8_uart.h"
#include <Wire.h>

#include "portable-co2-monitor.h"


S8_UART *sensor_S8;
S8_sensor sensor;

int max_co2 = 0;
double ave_co2 = 0.0;
unsigned long n_measurements = 0;

int last_measurement_timestamp = 0;

 // Set up BLE characteristics (randomly generated UUID, with 4 hex values modified)
BLEService co2Service("FA11B9C1-426C-485B-8184-03EC0E0741EC");
BLEIntCharacteristic co2CurLevel("FA116969-426C-485B-8184-03EC0E0741EC", BLERead | BLENotify);
BLEIntCharacteristic co2MaxLevel("FA116970-426C-485B-8184-03EC0E0741EC", BLERead | BLENotify);
BLEIntCharacteristic co2AvgLevel("FA116971-426C-485B-8184-03EC0E0741EC", BLERead | BLENotify);

void setup() {

  // Set up logging
  Serial.begin(9600);
  Serial.println("Starting up...");

  // Set up BLE
  while (!BLE.begin()) {
    Serial.println("Waiting for BLE to start");
    delay(1);
  }

  // set the local name that the peripheral advertises:
  BLE.setLocalName("BLE_CO2Monitor");
  // set the UUID for the service:
  BLE.setAdvertisedService(co2Service);

  // add the characteristic to the service
  co2Service.addCharacteristic(co2CurLevel);
  co2Service.addCharacteristic(co2MaxLevel);
  co2Service.addCharacteristic(co2AvgLevel);

  // add service
  BLE.addService(co2Service);

  // set read request handler for characteristics
  co2CurLevel.setEventHandler(BLERead, co2CurLevelCharacteristicRead);
  co2MaxLevel.setEventHandler(BLERead, co2MaxLevelCharacteristicRead);
  co2AvgLevel.setEventHandler(BLERead, co2AvgLevelCharacteristicRead);

  // start advertising the service
  BLE.advertise();

  // Initialize UART and S8 sensor
  Serial1.begin(S8_BAUDRATE);
  sensor_S8 = new S8_UART(Serial1); 

  // Check if S8 is available
  sensor_S8->get_firmware_version(sensor.firm_version);
  int len = strlen(sensor.firm_version);
  int n_tries = 1;
  while (len == 0 && n_tries < 120) {
      delay(250);
      sensor_S8->get_firmware_version(sensor.firm_version);
      len = strlen(sensor.firm_version);
      n_tries++; // give up after some number of tries
  }

  // Show basic S8 sensor info
  Serial.println("SenseAir S8 Firmware:");
  if (len == 0) Serial.print("N/A");
  else Serial.println(sensor.firm_version);
  sensor.sensor_id = sensor_S8->get_sensor_ID();

  // Show this device's bluetooth mac address
  Serial.println("Bluetooth MAC address:");
  BLEDevice central = BLE.central();
  Serial.print("BLE MAC Address: ");
  Serial.println(central.address());
}

void loop() {
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    // print the central's BT address:
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    // turn on LED to indicate connection:
    digitalWrite(LED_BUILTIN, HIGH);

    // while the central remains connected:
    while (central.connected()) {
      // Check if enough time has passed since last check
      if (millis() - last_measurement_timestamp < UPDATE_DELAY) {
        continue;
      }
      last_measurement_timestamp = millis();

      char buffer[17]; // to hold formated print

      // Get and display CO2 measure
      sensor.co2 = sensor_S8->get_co2();
      sprintf(buffer, "%4d", sensor.co2);
      Serial.println("CO2: " + String(buffer));

      // max since start-up
      if (sensor.co2 > max_co2) { max_co2 = sensor.co2; }
      sprintf(buffer, "%-4d", max_co2);
      Serial.println("Max: " + String(buffer));

      // average
      if(n_measurements == 0) {
          ave_co2 = (double) sensor.co2;
      } else {
          ave_co2 = (ave_co2 * (double)n_measurements + (double)sensor.co2) / (double)(n_measurements+1);
      }
      n_measurements++;
      int ave_co2_int = round(ave_co2);
      sprintf(buffer, "%4d", ave_co2_int);
      Serial.println("Ave: " + String(buffer));

      // Update BLE characteristics
      co2CurLevel.writeValue(sensor.co2);
      co2MaxLevel.writeValue(max_co2);
      co2AvgLevel.writeValue(ave_co2_int);
    }
  } else {
    // turn off the LED
    digitalWrite(LED_BUILTIN, LOW);
  }
}

/* Read request handler for characteristics */
void co2CurLevelCharacteristicRead(BLEDevice central, BLECharacteristic characteristic) {
  co2CurLevel.writeValue(sensor.co2);
}
void co2MaxLevelCharacteristicRead(BLEDevice central, BLECharacteristic characteristic) {
  co2MaxLevel.writeValue(max_co2);
}
void co2AvgLevelCharacteristicRead(BLEDevice central, BLECharacteristic characteristic) {
  co2AvgLevel.writeValue(round(ave_co2));
}