/*
 Custom temperature-sensor based thermostat that activates an output when
 the max temperature is exceeded. Can cycle through max temp options by 
 pressing a switch.
 
 Based on LCD library

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int backLight = 13;
const int activeWhenOverMaxTemp = 7;

const int temperatureSensorPin = A0;
const int lightSensorPin = A1;

int maxTemp = 70;
long maxTempPreviousMillis = 0; 

long readTempInterval = 1000;
long readTempPreviousMillis = 0; 

long maxTempSwitchInterval = 200;

int tempSwitchState = 0;

void setup() {
  pinMode(backLight, OUTPUT);
  digitalWrite(backLight, HIGH);

  pinMode(activeWhenOverMaxTemp, OUTPUT);
  digitalWrite(activeWhenOverMaxTemp, LOW);
  
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  String tempStr = String(maxTemp);
  String message = "max: " + tempStr;
  lcd.print(message);
}

void loop() {
  // turn backlight on or off, depending on read value of light sensor
  int lightSensorValue = analogRead(lightSensorPin);

  if (lightSensorValue > 700) {
    digitalWrite(backLight, HIGH);
  } else {
    digitalWrite(backLight, LOW);
  }
  
  unsigned long currentMillis = millis();

  if (currentMillis - maxTempPreviousMillis > maxTempSwitchInterval) {
    maxTempPreviousMillis = currentMillis;
    
    tempSwitchState = digitalRead(6);
    if (tempSwitchState == HIGH) {
      maxTemp++;
      if (maxTemp > 80) {
        maxTemp = 50;
      }
      
      lcd.setCursor(0, 0);
      String tempStr = String(maxTemp);
      String message = "max: " + tempStr;
      lcd.print(message);
    }
  }
  
  if (currentMillis - readTempPreviousMillis  > readTempInterval) {
    readTempPreviousMillis = currentMillis;  
     
    int sensorVal = analogRead(temperatureSensorPin);
  
    // convert the ADC reading to voltage
    float voltage = (sensorVal/1024.0) * 5.0;

    float temperature = (voltage - .5) * 100;
  
    float tempF = (temperature * 1.8) + 32;
    int tempInt = (int)tempF;
    String outputTemp = String(tempInt);
    String outMsg = "current: " + outputTemp;
    
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    lcd.print(outMsg);

    if (tempInt > (int)maxTemp) {
      digitalWrite(activeWhenOverMaxTemp, HIGH);
    } else {
      digitalWrite(activeWhenOverMaxTemp, LOW);
    }
  }
}
