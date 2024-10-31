//
//  ConnectViewModel.swift
//  iOSArduinoBLE
//
//  Authors: Andrea Finollo, Leonardo Cavagnis
//

import Foundation

final class ConnectViewModel: ObservableObject {
    @Published var state = State.idle
    
    var useCase: PeripheralUseCaseProtocol
    let connectedPeripheral: Peripheral
    
    init(useCase: PeripheralUseCaseProtocol,
         connectedPeripheral: Peripheral) {
        self.useCase = useCase
        self.useCase.peripheral = connectedPeripheral
        self.connectedPeripheral = connectedPeripheral
        self.setCallbacks()
    }
    
    private func setCallbacks() {
        useCase.onPeripheralReady = { [weak self] in
            self?.state = .ready
        }
        
        useCase.onReadTemperature = { [weak self] value in
            self?.state = .temperature(value)
        }
        
        useCase.onError = { error in
            print("Error \(error)")
        }
    }
    
    func startNotifyTemperature() {
        useCase.notifyTemperature(true)
    }
    
    func stopNotifyTemperature() {
        useCase.notifyTemperature(false)
    }
    
    func readTemperature() {
        useCase.readTemperature()
    }
}

extension ConnectViewModel {
    enum State {
        case idle
        case ready
        case temperature(Int)
    }
}
