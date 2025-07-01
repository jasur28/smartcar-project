# 🚗 SmartCar BLE-проект на ESP32 и Qt

Проект умной машинки на базе ESP32 с управлением по Bluetooth Low Energy (BLE) через мобильное приложение.

---

## 📱 Возможности

- Управление машинкой: Вперёд (F), Назад (B), Влево (L), Вправо (R), Стоп (S)
- Поддержка BLE (Bluetooth Low Energy)
- Сканирование и подключение к устройствам ESP32
- Простое и понятное мобильное приложение на Android

---

## 🧩 Технологии

| Компонент       | Используемая версия        |
|----------------|----------------------------|
| Микроконтроллер | ESP32-WROOM-32             |
| BLE-библиотека  | Arduino BLE (v3.2.0)       |
| IDE             | Arduino IDE 2.x            |
| Мобильное приложение | Qt 6.9.1 + QML        |
| Android SDK     | Android 31+ (API 31+)      |
| Поддержка BLE   | Через QtBluetooth (Qt6)    |

---

## ⚙ Подключение проводов к ESP32

| Назначение          | Пин ESP32                       |
| ------------------- | ------------------------------- |
| ENA (ШИМ мотор A)   | GPIO5                           |
| IN1 (направление A) | GPIO18                          |
| IN2 (направление A) | GPIO19                          |
| ENB (ШИМ мотор B)   | GPIO17                          |
| IN3 (направление B) | GPIO16                          |
| IN4 (направление B) | GPIO4                           |
| Питание ESP32       | VIN от драйвера моторов или USB |
| GND                 | Общий с драйвером моторов       |


---

📱 Системные требования и сборка Qt-приложения (Android)
Для сборки мобильного приложения использовались следующие компоненты:

Инструмент	Версия
- Qt	6.9.1
- Android NDK	27.2.12479018
- Build-tools	35.0.0
- Cmdline-tools	latest
- CMake	3.22.1
- Android Platforms	android-31, android-35

---

## 📂 Структура проекта
smartcar/
├── ESP-SMART-CAR/ # Код для ESP32 (Arduino)
│ └── ESP-SMART-CAR.ino
├── main.cpp
├── BluetoothManager.h
├── BluetoothManager.cpp
├── main.qml
├── CMakeLists.txt
├── .gitignore
└── README.md
