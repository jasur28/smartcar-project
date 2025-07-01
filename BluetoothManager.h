// ===== BluetoothManager.h =====
#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QBluetoothUuid>
#include <QCoreApplication>
#include <QBluetoothPermission>

class BluetoothManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList deviceNames READ deviceNames NOTIFY devicesChanged)

public:
    explicit BluetoothManager(QObject *parent = nullptr);
    QStringList deviceNames() const;

    Q_INVOKABLE void startScan();
    Q_INVOKABLE void connectToDevice(int index);
    Q_INVOKABLE void sendCommand(const QString &cmd);

signals:
    void devicesChanged();
    void statusMessage(const QString &msg);

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &info);
    void scanFinished();
    void controllerConnected();
    void controllerDisconnected();
    void controllerError(QLowEnergyController::Error error);
    void serviceDiscovered(const QBluetoothUuid &uuid);
    void serviceScanDone();
    void serviceStateChanged(QLowEnergyService::ServiceState newState);

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QList<QBluetoothDeviceInfo> devices;
    QStringList m_deviceNames;

    QLowEnergyController *controller = nullptr;
    QLowEnergyService *service = nullptr;
    QLowEnergyCharacteristic commandChar;

    const QBluetoothUuid SERVICE_UUID = QBluetoothUuid(QStringLiteral("12345678-1234-1234-1234-1234567890ab"));
    const QBluetoothUuid CHARACTERISTIC_UUID = QBluetoothUuid(QStringLiteral("abcd1234-5678-90ab-cdef-1234567890ab"));
};

#endif // BLUETOOTHMANAGER_H
