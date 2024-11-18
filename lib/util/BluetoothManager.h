
#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include <NimBLEDevice.h>

class BluetoothManager {
public:
    static BluetoothManager& getInstance();
    void begin();
    void sendStatusMessage(const String& message);

    bool isCommandAvailable();
    String getLastCommand(bool doNotClear = false);

private:
    BluetoothManager();
    class MyCallbacks : public NimBLECharacteristicCallbacks {
        void onWrite(NimBLECharacteristic* pCharacteristic) override;
    };

    NimBLEServer* pServer;
    NimBLEService* pService;
    NimBLECharacteristic* pTxCharacteristic;
    NimBLECharacteristic* pRxCharacteristic;

    String lastCommand;
};

#endif // BLUETOOTH_MANAGER_H