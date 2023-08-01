#include <ArduinoBLE.h>


#define DEBUG

const int BUFFER_SIZE = 100;
uint8_t buf[BUFFER_SIZE];

BLEDevice rightHaptic;
BLEDevice leftHaptic;
BLEDevice hapticDevice;

const uint8_t MESSAGE_LENGTH = 3;

BLECharacteristic leftMotorSender;
BLECharacteristic rightMotorSender;

const int leftDevice = 1;
const int rightDevice = 2;

const int leftMotor = 9;
const int middleMotor = 10;
const int rightMotor = 11;


uint8_t left [MESSAGE_LENGTH] = {leftMotor,254,5};
uint8_t middle [MESSAGE_LENGTH] = {middleMotor,254,5};
uint8_t right [MESSAGE_LENGTH] = {rightMotor,254,5};



void setup() {
  // putBLE.begin your setup code here, to run once:
  BLE.begin();
  Serial.begin(115200);
  while(!Serial)
  //while(!BLE.available()){}
  delay(2000);
  //BLE.debug(Serial);


  Serial.println("Scanning for devices");

  //BLE.scanForUuid("AFFE");
  //connect();


  connect();

  

}




void loop() {

  rightHaptic.connected();
  leftHaptic.connected();

  connect();

  if(Serial.available() > 0){
      uint8_t rlen = Serial.readBytesUntil(5, buf, BUFFER_SIZE);
  	  if (rlen != 3) {
        #ifdef DEBUG
        Serial.println("Something went wrong");
        #endif
        buf[0]=0;
      }
        switch(buf[0]){
          case leftDevice:
            sendCommand(leftMotorSender, buf[1], buf[2]);
            break;
          case rightDevice:
            sendCommand(rightMotorSender, buf[1], buf[2]);
            break;

        }

  }
  

  /*
  if(Serial.available() > 0){
    
  }*/

}



void sendCommand(BLECharacteristic sender, uint8_t selectedMotor, uint8_t intensity){


  switch(selectedMotor){
      case leftMotor:
        left[1]=intensity;
        for (int i = 0; i<MESSAGE_LENGTH;i++){
          sender.writeValue(left[i]);
        }
        break;
      case middleMotor:
        middle[1]=intensity;
        for (int i = 0; i<MESSAGE_LENGTH;i++){
          sender.writeValue(middle[i]);
        }
        break; 
      case rightMotor:
        right[1]=intensity;
        for (int i = 0; i<MESSAGE_LENGTH;i++){
          sender.writeValue(right[i]);
        }
        break;

    }
}





void connect(){
  bool isConnected;
  uint8_t connectedDevices;
  String connectedDeviceName;

  connectedDevices = BLE.peripheralCount();

  switch(connectedDevices){
    case 0:
      //BLE.scanForName("RIGHT");
      BLE.scanForUuid("ABBA");
      connectionloop(1);

      #ifdef DEBUG
      Serial.println("Connected to "+ BLE.peripheral(0).deviceName());
      #endif

      BLE.scanForUuid("AFFE");
      connectionloop(2);

      #ifdef DEBUG
      Serial.println("Connected to "+ BLE.peripheral(1).deviceName());
      #endif

      assignBLEDevices();
      getCharacteristic();

      break;
    case 1:
      connectedDeviceName = BLE.peripheral(0).deviceName();
      if (connectedDeviceName == "RIGHT"){
        BLE.scanForUuid("AFFE");

        #ifdef DEBUG
        Serial.println("Connecting to the left foot");
        #endif
      }
      else{
        BLE.scanForUuid("ABBA");
        #ifdef DEBUG
        Serial.println("Connecting to the right foot");
        #endif
      }
      connectionloop(2);
      assignBLEDevices();
      getCharacteristic();
      break;
  }

}


void connectionloop(int maxConnectedDevices){
  unsigned long currentMillis = millis();
  
  while (BLE.peripheralCount() < maxConnectedDevices) {
  //while (millis()-currentMillis <10000) {

    hapticDevice = BLE.available();
    if (hapticDevice) {
      BLE.stopScan();

      if (!hapticDevice.connect()) {
        #ifdef DEBUG
        Serial.println("Failed to connect!");
        #endif
        return;
      }

      if (!hapticDevice.discoverAttributes()) {
        #ifdef DEBUG
        Serial.println("Attribute discovery failed!");
        #endif
        hapticDevice.disconnect();
        return;
      }
    }
  }
}

void assignBLEDevices(){
  if(BLE.peripheral(0).deviceName() == "RIGHT"){
    rightHaptic = BLE.peripheral(0);
    leftHaptic = BLE.peripheral(1);
  }
  else{
    rightHaptic = BLE.peripheral(1);
    leftHaptic = BLE.peripheral(0);
  }
}

void getCharacteristic(){
  if (rightHaptic) {
    rightMotorSender = rightHaptic.characteristic("ABBB");

    if (!rightMotorSender) {
      #ifdef DEBUG
      Serial.println("rightMotor does not have motor control characteristic!");
      #endif
      rightHaptic.disconnect();
    }
  }
  if (leftHaptic) {
    leftMotorSender = leftHaptic.characteristic("AFFF");

    if (!leftMotorSender) {
      #ifdef DEBUG
      Serial.println("leftMotor does not have motor control characteristic!");
      #endif
      leftHaptic.disconnect();
    }
  }      


}
