#include <SoftwareSerial.h>

SoftwareSerial bluetooth(2,4);

#define DEBUG

const int BUFFER_SIZE = 100;
uint8_t buf[BUFFER_SIZE];


char command;


const uint8_t leftPin = 9;
const uint8_t middlePin = 10;
const uint8_t rightPin = 11;


void setup() {
  pinMode(leftPin,OUTPUT);
  analogWrite(leftPin,0);

  pinMode(middlePin,OUTPUT);
  analogWrite(middlePin,0);

  pinMode(rightPin,OUTPUT);
  analogWrite(rightPin,0);
  bluetooth.begin(9600);
  Serial.begin(9600);
  while(!Serial){}

}

void loop() {

  #ifndef DEBUG
  if (bluetooth.available() > 0) {
    // read the incoming bytes:
    uint8_t rlen = bluetooth.readBytesUntil(5, buf, BUFFER_SIZE);
  	if (rlen != 2) {buf[0]=0;}

    switch (buf[0]){
      case leftPin:
        analogWrite(leftPin,buf[1]);
        break;
      case middlePin:
        analogWrite(middlePin,buf[1]);
        break;  
      case rightPin:
        analogWrite(rightPin,buf[1]);
        break;
    }
    
    /*prints the received data
    Serial.print("I received: ");

    for(int i = 0; i < rlen; i++)
      Serial.print(buf[i],DEC);
    Serial.println();*/
    
  }
  #endif
  /*
  if (bluetooth.available() > 0){
    //response = bluetooth.readString();
    command = bluetooth.read();
    analogWrite(leftPin,command);
  }  */
  
  #ifdef DEBUG
  if(bluetooth.available()>0){

    command = bluetooth.read();
    if(command =='O'){
      Serial.println();
    }
    Serial.write(command);
  }
  
    if(Serial.available() > 0){
      bluetooth.write(Serial.read());
    }
  #endif  



}
