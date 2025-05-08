#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE

#include <ESP32Servo.h>
#include <DabbleESP32.h>

// Servo setup
Servo servoX;
Servo servoY;

// Motor A control pins (L298N)
#define IN1 27
#define IN2 26

void setup() {
  Serial.begin(115200);
  Dabble.begin("ESP32-Control");  // Bluetooth name shown in Dabble app

  // Attach servos for turret
  servoX.attach(12);    // Horizontal servo
  servoY.attach(14);   // Vertical servo

  // Set motor control pins as output
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  stopMotor();  // Motor off initially
}

void loop() {
  Dabble.processInput();  // Read data from Dabble app

  // === Turret Movement with Joystick ===
  int joyX = GamePad.getXaxisData();  // -7 to +7
  int joyY = GamePad.getYaxisData();  // -7 to +7

  int servoSpeedX = map(joyX, -7, 7, 0, 180);
  int servoSpeedY = map(joyY, -7, 7, 0, 180);

  if (abs(joyX) < 2) servoSpeedX = 90;
  if (abs(joyY) < 2) servoSpeedY = 90;

  servoX.write(servoSpeedX);
  servoY.write(servoSpeedY);

  // === Motor Control with Buttons ===
  if (GamePad.isCirclePressed()) {
    rotateClockwise();
  }
  else if (GamePad.isSquarePressed()) {
    rotateCounterClockwise();
  }
  else if (GamePad.isCrossPressed()) {  // 'X' button pressed
    stopMotor();  // Manual stop
  }
  else {
    stopMotor();  // Stop if no button is pressed
  }

  delay(50);  // Small delay for stability
}

// === Motor control functions ===
void rotateClockwise() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void rotateCounterClockwise() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}
