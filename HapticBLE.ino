#include <ArduinoBLE.h>


BLEService haptic("0xAFFE");
BLEUnsignedCharCharacteristic hapticCommand("0xAFFF", BLERead | BLEWrite); 
BLEDevice hapticDevice;

const uint8_t MESSAGE_LENGTH = 3;

BLECharacteristic batteryLevelChar;
uint8_t value = 0;
const int leftMotor = 3;
const int rightMotor = 5;


uint8_t left [MESSAGE_LENGTH] = {leftMotor,254,10};
uint8_t right [MESSAGE_LENGTH] = {rightMotor,254,10};



void setup() {
  // putBLE.begin your setup code here, to run once:
  BLE.begin();
  Serial.begin(115200);
  while(!Serial){}
  delay(2000);
  //BLE.debug(Serial);

  Serial.println("Scanning for devices");
  BLE.scanForUuid("AFFE");

  while (BLE.peripheralCount() < 1) {

    hapticDevice = BLE.available();
    if (hapticDevice) {
      BLE.stopScan();

      if (!hapticDevice.connect()) {
        Serial.println("Failed to connect!");
        return;
      }

      if (!hapticDevice.discoverAttributes()) {
        Serial.println("Attribute discovery failed!");
        hapticDevice.disconnect();
        return;
      }
    }
  }

  Serial.println("Connected");
  //Serial.print(hapticDevice.hasCharacteristic("AFFF"));
  batteryLevelChar = hapticDevice.characteristic("AFFF");

  
 


}

void loop() {

  BLEDevice peripheral = BLE.peripheral(0);
      if (peripheral) {
        BLECharacteristic batteryLevelChar = peripheral.characteristic("AFFF");

        if (!batteryLevelChar) {
          Serial.println("Peripheral does not have battery level characteristic!");
          peripheral.disconnect();
        }
      }   
  /* 
  delay(1000);
  left[1]=200;
  for (int i = 0; i<MESSAGE_LENGTH;i++){
    batteryLevelChar.writeValue(left[i]);
  }
  delay(1000);
  left[1]=100;
  for (int i = 0; i<MESSAGE_LENGTH;i++){
    batteryLevelChar.writeValue(left[i]);
  }*/

  //batteryLevelChar.writeValue(value);
  // put your main code here,  to run repeatedly:
  if(Serial.available() > 0){
    switch(Serial.read()){
      case 'L':
        left[1]=200;
        for (int i = 0; i<MESSAGE_LENGTH;i++){
          batteryLevelChar.writeValue(left[i]);
        }
        break;
      case 'O':
        left[1]=0;
        for (int i = 0; i<MESSAGE_LENGTH;i++){
          batteryLevelChar.writeValue(left[i]);
        }
        break;
      case 'R':
        right[1]=200;
        for (int i = 0; i<MESSAGE_LENGTH;i++){
          batteryLevelChar.writeValue(right[i]);
        }
        break;
        case 'F':
        right[1]=0;
        for (int i = 0; i<MESSAGE_LENGTH;i++){
          batteryLevelChar.writeValue(right[i]);
        }
        break;
    }
  }

}
