#include <Servo.h>

#define DEBUG_MODE true

Servo servoObject;

#define SERVOPIN 10
#define POTPIN 0
#define POTMIN 560
#define POTMAX 1020
#define SERVOMIN 30
#define SERVOMAX 150

byte lastPotValue;
byte lastWrittenPotValue;

int goalPosition = 127;
int servoCommand;

byte receivedPositionCommand = 0;

void setup()
{
  Serial.begin(9600);
  servoObject.attach(SERVOPIN);
}

void loop()
{
  byte goalPosition;
  int rawPotValue = constrain(analogRead(POTPIN), POTMIN, POTMAX);
  byte potValue = map(rawPotValue, POTMIN, POTMAX, 0, 255);
  int clonePosition = map(potValue, 0, 255, SERVOMIN, SERVOMAX);

  // DEBUG:
  if (DEBUG_MODE) {
    goalPosition = potValue;
    commandServo(goalPosition);

    Serial.print("P: ");
    Serial.print(rawPotValue);
    Serial.print("(");
    Serial.print(byte(potValue), DEC);
    Serial.print("), M: ");
    Serial.println(clonePosition);
    delay(50);

    // NORMAL:
  } else {
    while (Serial.available() > 0) {
      goalPosition = Serial.read();
      commandServo(goalPosition);
    }
    // only write if changed.
    // TODO: check for out by one as well.
    if (abs(potValue - lastWrittenPotValue) > 0x02) {
      Serial.write(potValue);
      lastWrittenPotValue = potValue;
    }
    lastPotValue = potValue;
  }
}

/// Move the servo in response to input bytes
void commandServo(byte goal)
{
  servoCommand = constrain(map(goal, 0, 255, SERVOMIN, SERVOMAX), SERVOMIN, SERVOMAX);
  servoObject.write(servoCommand);
}
