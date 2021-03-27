#include <Arduino.h>
#include <Ticker.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>
#define USE_SERIAL Serial

// Connect the port of the stepper motor driver
int outPorts[] = {14, 27, 26, 25};
bool stepper = true;

Servo myservo;
int servoPin = 15;
int posVal = 0;
int valAdd = 2;

Ticker stepperTimer;
Ticker servoTimer;

void setup() {
  // set pins to output
  for (int i = 0; i < 4; i++) {
    pinMode(outPorts[i], OUTPUT);
  }
  myservo.setPeriodHertz(50);
  myservo.attach(servoPin, 500, 2500);

    USE_SERIAL.begin(115200);
  const char *ssid_Router     = "Columbia University"; //Enter the router name
  WiFi.begin(ssid_Router);
  USE_SERIAL.println(String("Connecting to ")+ssid_Router);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    USE_SERIAL.print(".");
  }
  USE_SERIAL.println("\nConnected, IP address: ");
  USE_SERIAL.println(WiFi.localIP());
  USE_SERIAL.println("Setup End");
  
}

String address= "http://165.227.76.232:3000/clj2142/running";

void loop(){
  
  if((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin(address);
 
    int httpCode = http.GET(); // start connection and send HTTP header
    if (httpCode == HTTP_CODE_OK) { 
        String response = http.getString();
        if (response.equals("false")) {
            stepperTimer.attach_ms(8000, nothing);
            servoTimer.attach_ms(8000, nothing);
            delay(5);
        }
        else if(response.equals("true")) {
            stepperTimer.attach_ms(500, stepperTurn);
            servoTimer.attach_ms(50, servoTurn);
        }
        USE_SERIAL.println("Response was: " + response);
    } else {
        USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
    delay(500); // sleep for half of a second
  }
}

void stepperTurn(){
  if (stepper == true){
    moveSteps(true, random(0,7) * 64, random(1,4));
    stepper = false;
  }
  else{
    moveSteps(false, random(0,7) * 64, random(1,4));
    stepper = true;
  }
}

void moveOneStep(bool dir) {
// Define a variable, use four low bit to indicate the state of port 
  static byte out = 0x01;
// Decide the shift direction according to the rotation direction
if (dir) { // ring shift left
  out!=0x08? out=out<<1:out=0x01; }
else { // ring shift right
  out!=0x01? out=out>>1:out=0x08;
}
for (int i = 0; i < 4; i++) {
digitalWrite(outPorts[i], (out & (0x01 << i)) ? HIGH : LOW);
} }

void moveSteps(bool dir, int steps, byte ms) {
  for (unsigned long i = 0; i < steps; i++) {
    moveOneStep(dir); // Rotate a step
    delay(constrain(ms,3,20));        // Control the speed
  }
}

void servoTurn(){
      myservo.write(posVal);
      if (posVal == 0){
        valAdd = 2;
      }
      else if (posVal == 90){
        posVal = -2;
      }
      posVal += valAdd;
}

void nothing(){
}
