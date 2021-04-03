// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  //Wire.setClock(50);
}

void loop() {

  //sander
  delay(500);
  Wire.requestFrom(B0010011, 4,true);   

  while (Wire.available()) { // slave may send less than requested
    int c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
    Serial.print(" "); 
  }
  Serial.println();

  //leen
  delay(5000);
  Wire.requestFrom(B0001000, 2,true);   

  while (Wire.available()) { // slave may send less than requested
    int c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
    Serial.print(" "); 
  }
  Serial.println();

}
