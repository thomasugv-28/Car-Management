/*
Arduino Smart Parking System
Cleaned & Fixed Version
Based on Simple Circuits Project
*/

#include<wire.h>
#include<LiquidCrystal_I2C.h>
#include<servo.h>

/* ------------------- LCD ------------------- */
LiquidCrystal_I2C lcd(0x27, 16, 2);

/* ------------------- SERVO ----------------- */
Servo gateServo;


const int SERVO_PIN = 4;
const int GATE_OPEN = 0;
const int GATE_CLOSE = 100;

/* ------------------- IR SENSORS ------------ */
const int IR_ENTRY = 2;
const int IR_EXIT = 3;

/* ------------------- PARKING --------------- */
int slotsAvailable = 4; // Total parking slots

bool entryDetected = false;
bool exitDetected = false;

/* ------------------- SETUP ----------------- */
void setup() {
Serial.begin(9600);

pinMode(IR_ENTRY, INPUT);
pinMode(IR_EXIT, INPUT);

gateServo.attach(SERVO_PIN);
gateServo.write(GATE_CLOSE);

lcd.init();
lcd.backlight();

lcd.setCursor(0, 0);
lcd.print(" ARDUINO ");
lcd.setCursor(0, 1);
lcd.print(" PARKING SYSTEM ");
delay(2000);
lcd.clear();
}

/* ------------------- LOOP ------------------ */
void loop() {

/* -------- ENTRY SENSOR -------- */
if (digitalRead(IR_ENTRY) == LOW && !entryDetected) {
entryDetected = true;

if (slotsAvailable > 0) {
if (!exitDetected) {
gateServo.write(GATE_OPEN);
slotsAvailable--;
}
} else {
lcd.clear();
lcd.setCursor(0, 0);
lcd.print(" SORRY :( ");
lcd.setCursor(0, 1);
lcd.print(" Parking Full ");
delay(3000);
lcd.clear();
}
}

/* -------- EXIT SENSOR -------- */
if (digitalRead(IR_EXIT) == LOW && !exitDetected) {
exitDetected = true;

if (!entryDetected) {
gateServo.write(GATE_OPEN);
slotsAvailable++;
}
}

/* -------- RESET AFTER PASS -------- */
if (entryDetected && exitDetected) {
delay(1000);
gateServo.write(GATE_CLOSE);
entryDetected = false;
exitDetected = false;
}

/* -------- LCD DISPLAY ---------- */
lcd.setCursor(0, 0);
lcd.print(" WELCOME! ");
lcd.setCursor(0, 1);
lcd.print("Slot Left: ");
lcd.print(slotsAvailable);
lcd.print(" ");
}
