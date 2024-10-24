/*
* Name: clock and temp project
* Author: Victor Huke
* Date: 2024-10-10
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display,
* Further, it measures temprature with a analog temprature module and displays a mapped value to a 9g-servo-motor
*/

// Include Libraries
#include <RTClib.h>
#include <Wire.h>
#include <Servo.h>
#include "U8glib.h"

// Init constants
const int tempPin = A0;
const int redPin= 3;
const int greenPin = 5;
const int bluePin = 11;
// Init global variables

// construct objects
RTC_DS3231 rtc;
Servo myservo;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
void setup() {
  // init communication
  Serial.begin(9600);

  Wire.begin();  //clock communication startup
  rtc.begin();

  pinMode(tempPin, INPUT);  //tempsensor pin
  // Init Hardware

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  myservo.attach(9);  //servo initialisation

  u8g.setFont(u8g_font_unifont);  //sätter font för skärmen
  //u8g.firstPage();
  //rensar skärmen
 /* 
  pinMode(redPin,  OUTPUT);              
  pinMode(greenPin, OUTPUT);  //intialiserar pinarna för rgb lampan
  pinMode(bluePin, OUTPUT);
  */
}

void loop() {


  //servoWrite(getTemp());
  servoWrite(map(getTemp(), 20.0,40,0,180));
  Serial.println(getTime() + "     " + getTemp());
  //oledWrite(0,40,getTime());
  oledWrite(0,40,String(String(getTemp()) + "  " + getTime()));
  delay(100);
  setColor(255, 255, 0);
  
}


/*
*This function reads time from an ds3231 module and package the time as a String
*Parameters: Void
*Returns: time in hh:mm:ss as String
*/
String getTime() {
  DateTime now = rtc.now();
  String currenttime = (String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second()));
  return currenttime;
}

/*
* This function reads an analog pin connected to an analog temprature sensor and calculates the corresponding temp
*Parameters: Void
*Returns: temprature as float
*/
float getTemp() {
  float temp = 0;
  float R1 = 10000;  // value of R1 on board
  float logR2, R2, T;
  float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;  //steinhart-hart coeficients for thermistor

  int Vo = analogRead(tempPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);  //calculate resistance on thermistor
  logR2 = log(R2);
  temp = (1.0 / (c1 + c2 * logR2 + c3 * logR2 * logR2 * logR2));  // temperature in Kelvin
  temp = temp - 273.15;                                           //convert Kelvin to Celcius

  return temp;
}

/*
* This function takes a string and draws it to an oled display
*Parameters: - text: String to write to display
*Returns: void
*/
void oledWrite(int x, int y, String text) {
  //u8g.nextPage();
  //u8g.drawStr(20,20, text.c_str());
  u8g.firstPage();
 
  do {
    u8g.drawStr(x, y, text.c_str());

  } while (u8g.nextPage());
}

/*
* takes a temprature value and maps it to corresppnding degree on a servo
*Parameters: - value: temprature
*Returns: void
*/
void servoWrite(float angle) {
  myservo.write(angle);
}
/*
takes a rgb value and sends that color to a rgb led
parameters: -value: red 0-255, green 0-255, blue 0-255
returns: void
*/
void setColor(int redValue, int greenValue,  int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin,  greenValue);
  analogWrite(bluePin, blueValue);
}