#include <ArduinoBLE.h>

#define MEASUREMENT_INTERVAL 5000
#define SAFE_LEVEL  1000
#define BAD_LEVEL   1500

/* Read request handler for characteristics */
void co2CurLevelCharacteristicRead(BLEDevice central, BLECharacteristic characteristic);
void co2MaxLevelCharacteristicRead(BLEDevice central, BLECharacteristic characteristic);
void co2AvgLevelCharacteristicRead(BLEDevice central, BLECharacteristic characteristic);