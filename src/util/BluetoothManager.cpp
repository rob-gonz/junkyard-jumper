#include "BluetoothManager.h"
#include <Arduino.h>

BluetoothManager::BluetoothManager() : pServer(nullptr), pService(nullptr), pTxCharacteristic(nullptr), pRxCharacteristic(nullptr) {}

void BluetoothManager::begin() {
    NimBLEDevice::init("Junkyard-Jumper E-Bay");
    pServer = NimBLEDevice::createServer();
    pService = pServer->createService(NimBLEUUID((uint16_t)0x1101)); // SPP Service UUID

    pTxCharacteristic = pService->createCharacteristic(
        NimBLEUUID((uint16_t)0x2A37), // SPP TX Characteristic UUID
        NIMBLE_PROPERTY::NOTIFY
    );

    pRxCharacteristic = pService->createCharacteristic(
        NimBLEUUID((uint16_t)0x2A38), // SPP RX Characteristic UUID
        NIMBLE_PROPERTY::WRITE
    );

    pRxCharacteristic->setCallbacks(new MyCallbacks());
    pService->start();

    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(pService->getUUID());
    pAdvertising->setScanResponse(true);
    pAdvertising->start();

    // Serial.println("Bluetooth device is now advertising");
}

void BluetoothManager::sendStatusMessage(const std::string& message) {
    if (pTxCharacteristic) {
        pTxCharacteristic->setValue(message);
        pTxCharacteristic->notify();
    }
}

void BluetoothManager::MyCallbacks::onWrite(NimBLECharacteristic* pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    Serial.println("Received command: " + String(value.c_str()));
    // Handle received command

    //TODO: Whatever my State Manager eventually ends up being we'll copy the command to the state manager here
}