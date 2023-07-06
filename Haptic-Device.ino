#include <SoftwareSerial.h>

SoftwareSerial bluetooth(8,9);

const int BUFFER_SIZE = 100;
uint8_t buf[BUFFER_SIZE];


char command;
String response;
bool newWord = false;

const uint8_t leftPin = 3;
const uint8_t rightPin = 5;

uint8_t test;

void setup() {
  pinMode(leftPin,OUTPUT);
  analogWrite(leftPin,LOW);
  pinMode(rightPin,OUTPUT);
  digitalWrite(rightPin,LOW);
  bluetooth.begin(9600);
  Serial.begin(9600);
  while(!Serial){}

}

void loop() {
  if (bluetooth.available() > 0) {
    // read the incoming bytes:
    uint8_t rlen = bluetooth.readBytesUntil(10, buf, BUFFER_SIZE);
  	if (rlen != 2) {buf[0]=0;}

    switch (buf[0]){
      case leftPin:
        analogWrite(leftPin,buf[1]);
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
  
  /*
  if (bluetooth.available() > 0){
    //response = bluetooth.readString();
    command = bluetooth.read();
    analogWrite(leftPin,command);
  }  */
  

  /*if(bluetooth.available() > 0){
    Serial.write(bluetooth.read());
  }*/
  if(Serial.available() > 0){
    bluetooth.write(Serial.read());
  }



}
