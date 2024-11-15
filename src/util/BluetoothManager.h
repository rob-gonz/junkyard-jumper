
#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include <NimBLEDevice.h>

class BluetoothManager {
public:
    BluetoothManager();
    void begin();
    void sendStatusMessage(const std::string& message);

private:
    class MyCallbacks : public NimBLECharacteristicCallbacks {
        void onWrite(NimBLECharacteristic* pCharacteristic) override;
    };

    NimBLEServer* pServer;
    NimBLEService* pService;
    NimBLECharacteristic* pTxCharacteristic;
    NimBLECharacteristic* pRxCharacteristic;
};

#endif // BLUETOOTH_MANAGER_H