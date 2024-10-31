//
//  UUIDs.swift
//  iOSArduinoBLE
//
//  Authors: Andrea Finollo, Leonardo Cavagnis
//

import Foundation
import CoreBluetooth

enum UUIDs {
    static let ledService = CBUUID(string: "8370846E-2189-44BC-86F9-76FD3772BA84")
    static let ledStatusCharacteristic = CBUUID(string:  "83706969-2189-44BC-86F9-76FD3772BA84") // Write
    
    static let sensorService = CBUUID(string: "FA11B9C1-426C-485B-8184-03EC0E0741EC")
    static let temperatureCharacteristic = CBUUID(string:  "FA116969-426C-485B-8184-03EC0E0741EC") // Read | Notify
}
