#include "BluetoothManager.h"
#include <Arduino.h>


BluetoothManager& BluetoothManager::getInstance() {
    static BluetoothManager instance;
    return instance;
}

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

void BluetoothManager::sendStatusMessage(const String& message) {
    if (pTxCharacteristic) {
        pTxCharacteristic->setValue(message);
        pTxCharacteristic->notify();
    }
}

void BluetoothManager::MyCallbacks::onWrite(NimBLECharacteristic* pCharacteristic) {
    String value = pCharacteristic->getValue();
    // Serial.println("Received command: " + String(value.c_str()));
    BluetoothManager::getInstance().lastCommand = value;
}

String BluetoothManager::getLastCommand(bool doNotClear) {
    String command = lastCommand;
    if (!doNotClear) lastCommand = "";
    return command;
}

bool BluetoothManager::isCommandAvailable() {
    return lastCommand.length() > 0;
}