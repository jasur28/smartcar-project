#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Motor pins
const int ENA = 5;
const int IN1 = 18;
const int IN2 = 19;

const int ENB = 17;
const int IN3 = 16;
const int IN4 = 4;

// BLE UUIDs
#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890ab"
#define CHARACTERISTIC_UUID "abcd1234-5678-90ab-cdef-1234567890ab"

BLECharacteristic *pCharacteristic;
BLEServer *pServer;

void moveForward() {
    digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void moveBackward() {
    digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
  
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void turnRight() {
  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 200);
  analogWrite(ENB, 200);
}

void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// Handle BLE writes
class CommandCallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pChar) override {
    String value = String(pChar->getValue().c_str());
    if (value.isEmpty()) return;


    char command = toupper(value.charAt(0));
    Serial.print("Received command: ");
    Serial.println(command);

    switch (command) {
      case 'F': moveForward(); break;
      case 'B': moveBackward(); break;
      case 'L': turnLeft(); break;
      case 'R': turnRight(); break;
      case 'S': stopCar(); break;
      default: Serial.println("Unknown command"); break;
    }
  }
};

// Auto-advertise after disconnect
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *server) override {
    Serial.println("Client connected.");
  }

  void onDisconnect(BLEServer *server) override {
    Serial.println("Client disconnected. Restarting advertising...");
    BLEDevice::startAdvertising();
  }
};

void setup() {
  Serial.begin(115200);

  // Motor pins setup
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  stopCar(); // Initial stop

  BLEDevice::init("ESP32-SMARTCAR");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pCharacteristic->setCallbacks(new CommandCallback());
  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("ESP32 BLE Smart Car is ready!");
}

void loop() {
  // Nothing to do
}
