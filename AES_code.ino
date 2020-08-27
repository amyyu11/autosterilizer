#include <Servo.h>                 // include servo library
Servo myservo;                     // create servo object

#define potPin 0                   // analog potentiometer input
#define irPin 8                    // digital IR sensor input
#define servoPin 5                 // digtal PWM servo control pin
#define transistorPin 3            // digital transistor control output

int maxTime = 3000;                // longest time interval between IR reads (ms)
int minTime = 1350;                // shortest time interval between IR reads (ms)
int servoSpeed = 30;               // control servo speed
unsigned long time_now = millis(); // enable delay between spray

void setup() {
  pinMode(irPin, INPUT);           // IR sensor input
  pinMode(transistorPin, OUTPUT);  // transistor output
  myservo.attach(servoPin);        // attach servo
}

void loop() {
  int potDelay = analogRead(potPin);                     // read delay
  potDelay = map(potDelay, 0, 1023, minTime, maxTime) ;  // map delay to time scale
  
  if (potDelay <= minTime + 50) {}                       // do nothing (potentiometer turned off)
  else if (!digitalRead(irPin) && (millis() > (time_now + potDelay))) { // object present, delay satisfied
    time_now = millis();                                 // update delay for next spray
    digitalWrite(transistorPin, HIGH);                   // turn on transistor, allow current through servo
    delay(50);                                   // ensure steady state transistor performance
    
    for (int pos = 0; pos <= 180; pos += 10) {   // move servo from 0 to 180
      myservo.write(pos);                        // move servo
      delay(servoSpeed);                         // control servo speed/allow higher torque
    }
    delay(200);                                  // allow servo to fully complete move
    myservo.write(0);                            // move the motor back to 0 without delay
    delay(500);                                  // allow servo to fully complete move
    
    digitalWrite(transistorPin, LOW);            // turn off transistor, block current through servo
  }
}
