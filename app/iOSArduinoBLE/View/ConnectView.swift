//
//  ConnectView.swift
//  iOSArduinoBLE
//
//  Authors: Andrea Finollo, Leonardo Cavagnis
//

import SwiftUI
import UIKit

struct ConnectView: View {
    
    @ObservedObject var viewModel: ConnectViewModel
    
    @Environment(\.dismiss) var dismiss
    
    @State var isToggleOn: Bool = false
    @State var isPeripheralReady: Bool = false
    @State var lastTemperature: Int = 0
    
    // Define thresholds and constants for background colors
    private let minTemperature: Double = 400
    private let maxTemperature: Double = 1500
    private let opacityValue: Double = 0.2

    // Computed property to calculate the background color
    private var backgroundColor: Color {
        let scaledTemperature = min(max((Double(lastTemperature) - minTemperature) / (maxTemperature - minTemperature), 0.0), 1.0)
        let redComponent = scaledTemperature
        let greenComponent = 1.0 - scaledTemperature

        return Color(red: redComponent, green: greenComponent, blue: 0.0).opacity(opacityValue)
    }

    var body: some View {
        VStack {
            Text(viewModel.connectedPeripheral.name ?? "Unknown")
                .font(.title)
            Spacer()
                .frame(height: 20) // Adjust the height to control the vertical spacing between sections
            VStack {
                Text("\(lastTemperature) ppm")
                    .font(.largeTitle)
                HStack {
                    Toggle("Notify", isOn: $isToggleOn)
                        .disabled(!isPeripheralReady)
                        .padding(.trailing, 10)
                    Button("READ") {
                        viewModel.readTemperature()
                    }
                    .disabled(!isPeripheralReady)
                    .buttonStyle(.borderedProminent)
                }
            }
            .padding()
            Spacer()
                .frame(maxHeight:.infinity)
            Button {
                dismiss()
            } label: {
                Text("Disconnect")
                    .frame(maxWidth: .infinity)
            }
            .buttonStyle(.borderedProminent)
            .padding(.horizontal)
        }
        .background(backgroundColor)
        .onChange(of: isToggleOn) { newValue in
            if newValue == true {
                viewModel.startNotifyTemperature()
            } else {
                viewModel.stopNotifyTemperature()
            }
        }
        .onReceive(viewModel.$state) { state in
            switch state {
            case .ready:
                isPeripheralReady = true
            case let .temperature(temp):
                lastTemperature = temp
            default:
                print("Not handled")
            }
        }
    }
}

struct PeripheralView_Previews: PreviewProvider {
    
    final class FakeUseCase: PeripheralUseCaseProtocol {
        
        var peripheral: Peripheral?
        
        var onReadTemperature: ((Int) -> Void)?
        var onPeripheralReady: (() -> Void)?
        var onError: ((Error) -> Void)?
        
        func readTemperature() {
            onReadTemperature?(25)
        }
        
        func notifyTemperature(_ isOn: Bool) {}
    }

    static var viewModel = {
        ConnectViewModel(useCase: FakeUseCase(),
                            connectedPeripheral: .init(name: "iOSArduinoBoard"))
    }()
    
    
    static var previews: some View {
        ConnectView(viewModel: viewModel, isPeripheralReady: true)
    }
}

struct CardView: View {
  var body: some View {
    RoundedRectangle(cornerRadius: 16, style: .continuous)
      .shadow(color: Color(white: 0.5, opacity: 0.2), radius: 6)
      .foregroundColor(.init(uiColor: .secondarySystemBackground))
  }
}

