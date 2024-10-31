//
//  PeripheralUseCase.swift
//  iOSArduinoBLE
//
//  Authors: Andrea Finollo, Leonardo Cavagnis
//

import Foundation
import CoreBluetooth

protocol PeripheralUseCaseProtocol {
    
    var peripheral: Peripheral? { get set }

    var onReadTemperature: ((Int) -> Void)? { get set }
    var onPeripheralReady: (() -> Void)? { get set }
    var onError: ((Error) -> Void)? { get set }

    func readTemperature()
    func notifyTemperature(_ isOn: Bool)
}

class PeripheralUseCase: NSObject, PeripheralUseCaseProtocol {
    
    var peripheral: Peripheral? {
        didSet {
            self.peripheral?.cbPeripheral?.delegate = self
            discoverServices()
        }
    }
    
    var cbPeripheral: CBPeripheral? {
        peripheral?.cbPeripheral
    }
    
    var onReadTemperature: ((Int) -> Void)?
    var onPeripheralReady: (() -> Void)?
    var onError: ((Error) -> Void)?
    
   
    var discoveredServices = [CBUUID : CBService]()
    var discoveredCharacteristics = [CBUUID : CBCharacteristic]()
    
    func discoverServices() {
        cbPeripheral?.discoverServices([UUIDs.ledService, UUIDs.sensorService])
    }
    
    func readTemperature() {
        guard let tempCharacteristic = discoveredCharacteristics[UUIDs.temperatureCharacteristic] else {
            return
        }
        cbPeripheral?.readValue(for: tempCharacteristic)
    }
    
    func notifyTemperature(_ isOn: Bool) {
        guard let tempCharacteristic = discoveredCharacteristics[UUIDs.temperatureCharacteristic] else {
            return
        }
        cbPeripheral?.setNotifyValue(isOn, for: tempCharacteristic)
    }
    
    
    fileprivate func requiredCharacteristicUUIDs(for service: CBService) -> [CBUUID] {
        switch service.uuid {
        case UUIDs.ledService where discoveredCharacteristics[UUIDs.ledStatusCharacteristic] == nil:
            return [UUIDs.ledStatusCharacteristic]
        case UUIDs.sensorService where discoveredCharacteristics[UUIDs.temperatureCharacteristic] == nil:
            return [UUIDs.temperatureCharacteristic]
        default:
            return []
        }
    }
}

extension PeripheralUseCase: CBPeripheralDelegate {
    
    func peripheral(_ peripheral: CBPeripheral, didDiscoverServices error: Error?) {
        guard let services = peripheral.services, error == nil else {
            return
        }
        for service in services {
            discoveredServices[service.uuid] = service
            let uuids = requiredCharacteristicUUIDs(for: service)
            guard !uuids.isEmpty else {
                return
            }
            peripheral.discoverCharacteristics(uuids, for: service)
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didDiscoverCharacteristicsFor service: CBService, error: Error?) {
        guard let characteristics = service.characteristics else {
            return
        }
        for characteristic in characteristics {
            discoveredCharacteristics[characteristic.uuid] = characteristic
        }

        if discoveredCharacteristics[UUIDs.temperatureCharacteristic] != nil &&
            discoveredCharacteristics[UUIDs.ledStatusCharacteristic] != nil {
            onPeripheralReady?()
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didWriteValueFor characteristic: CBCharacteristic, error: Error?) {
        if let error {
            onError?(error)
            return
        }
        switch characteristic.uuid {
        default:
            fatalError()
        }
    }
    
    func peripheral(_ peripheral: CBPeripheral, didUpdateValueFor characteristic: CBCharacteristic, error: Error?) {
        switch characteristic.uuid {
        case UUIDs.temperatureCharacteristic:
            let value: UInt32 = {
                guard let data = characteristic.value else {
                    return 0
                }
                return data.withUnsafeBytes { $0.load(as: UInt32.self) }
            }()
            onReadTemperature?(Int(value))
        default:
            fatalError()
        }
    }
}
