#include "BluetoothManager.h"
#include <QDebug>

BluetoothManager::BluetoothManager(QObject *parent)
    : QObject(parent),
    discoveryAgent(new QBluetoothDeviceDiscoveryAgent(this)) {

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BluetoothManager::deviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BluetoothManager::scanFinished);
}

QStringList BluetoothManager::deviceNames() const {
    return m_deviceNames;
}

void BluetoothManager::startScan() {
    // 🔐 Request Bluetooth permission
    QBluetoothPermission permission;
    permission.setCommunicationModes(QBluetoothPermission::Access);
    if (qApp->checkPermission(permission) != Qt::PermissionStatus::Granted) {
        qApp->requestPermission(permission, [](const QPermission &result) {
            if (result.status() != Qt::PermissionStatus::Granted) {
                qWarning() << "Bluetooth permission not granted!";
            }
        });
    }

    devices.clear();
    m_deviceNames.clear();
    emit devicesChanged();
    discoveryAgent->start();
    emit statusMessage("🔍 Сканирование устройств...");
}

void BluetoothManager::connectToDevice(int index) {
    if (index < 0 || index >= devices.size()) return;

    if (controller) {
        controller->disconnectFromDevice();
        controller->deleteLater();
    }

    controller = QLowEnergyController::createCentral(devices[index], this);
    connect(controller, &QLowEnergyController::connected, this, &BluetoothManager::controllerConnected);
    connect(controller, &QLowEnergyController::disconnected, this, &BluetoothManager::controllerDisconnected);
    connect(controller, &QLowEnergyController::serviceDiscovered, this, &BluetoothManager::serviceDiscovered);
    connect(controller, &QLowEnergyController::discoveryFinished, this, &BluetoothManager::serviceScanDone);
    connect(controller, &QLowEnergyController::errorOccurred, this, &BluetoothManager::controllerError);

    controller->connectToDevice();
    emit statusMessage("🔗 Подключение к устройству...");
}

void BluetoothManager::sendCommand(const QString &cmd) {
    if (service && commandChar.isValid()) {
        QByteArray data = cmd.toUtf8();
        service->writeCharacteristic(commandChar, data, QLowEnergyService::WriteWithoutResponse);
        emit statusMessage("📤 Отправлено: " + cmd);
    } else {
        emit statusMessage("⚠️ Не готово к отправке");
    }
}

void BluetoothManager::deviceDiscovered(const QBluetoothDeviceInfo &info) {
    devices.append(info);
    m_deviceNames.append(info.name() + " [" + info.address().toString() + "]");
    emit devicesChanged();
}

void BluetoothManager::scanFinished() {
    emit statusMessage("✅ Поиск завершён");
}

void BluetoothManager::controllerConnected() {
    emit statusMessage("✅ Подключено! Поиск сервисов...");
    controller->discoverServices();
}

void BluetoothManager::controllerDisconnected() {
    emit statusMessage("🔌 Устройство отключено");
}

void BluetoothManager::controllerError(QLowEnergyController::Error error) {
    Q_UNUSED(error)
    emit statusMessage("❌ Ошибка Bluetooth-соединения");
}

void BluetoothManager::serviceDiscovered(const QBluetoothUuid &uuid) {
    if (uuid == SERVICE_UUID) {
        emit statusMessage("📡 Сервис найден");
    }
}

void BluetoothManager::serviceScanDone() {
    service = controller->createServiceObject(SERVICE_UUID, this);
    if (!service) {
        emit statusMessage("❌ Сервис не найден");
        return;
    }

    connect(service, &QLowEnergyService::stateChanged,
            this, &BluetoothManager::serviceStateChanged);
    service->discoverDetails();
}

void BluetoothManager::serviceStateChanged(QLowEnergyService::ServiceState newState) {
    if (newState == QLowEnergyService::ServiceDiscovered) {
        commandChar = service->characteristic(CHARACTERISTIC_UUID);
        if (commandChar.isValid()) {
            emit statusMessage("✅ Устройство готово к управлению");
        } else {
            emit statusMessage("❌ Характеристика не найдена");
        }
    }
}
