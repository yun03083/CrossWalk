#include <pitches.h>
#include <Servo.h>
#include <HX711.h>

#define calibration_factor 100.0
#define DOUT1 49
#define CLK1 48
#define DOUT2 53
#define CLK2 52

HX711 scale1(DOUT1, CLK1);
HX711 scale2(DOUT2, CLK2);

int RGB[] = {3, 4, 5};
int LED = 2;

int speakerPin = 57;
int melody = NOTE_G4;
int noteDuration = 4;
int sig_ped = 0;

Servo myServo;
int servoPin = 11;

unsigned long time_previous, time_current;
unsigned long interval = 2000;
boolean LED_state = false;
void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);
  scale1.set_scale(calibration_factor);
  scale1.tare();
  scale2.set_scale(calibration_factor);
  scale2.tare();
  for (int i = 0; i < 3; i++) {
    pinMode(RGB[i], OUTPUT);
  }
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LED_state);
  time_previous = millis();
}

void loop() {
  Serial.print("scale1: ");
  Serial.println(scale1.get_units(),1);
  Serial.print("scale2: ");
  Serial.println(scale2.get_units(),1);
  time_current = millis();
  if (time_current - time_previous >= interval) {
    time_previous = time_current;
    LED_state = !LED_state;
  }
  if(!LED_state) {
    digitalWrite(RGB[0], LOW);
    digitalWrite(RGB[1], HIGH);
    digitalWrite(RGB[2], HIGH);
  }
  
  if( LED_state ) {
    digitalWrite(RGB[0], HIGH);
    digitalWrite(RGB[1], LOW);
    digitalWrite(RGB[2], HIGH);
    while( scale1.get_units()>40||scale1.get_units()<-50 ){
      Serial.print("scale1: ");
      Serial.println(scale1.get_units(),1);
      Serial.print("scale2: ");
      Serial.println(scale2.get_units(),1);
      if(scale2.get_units()>30||scale2.get_units()<-50)
        sig_ped++;
      if(sig_ped%2==1){
        myServo.write(-90);
        tone(57, 1000);
        digitalWrite(LED, HIGH);
        delay(3000);
        myServo.write(90);
        noTone(57);
        digitalWrite(LED, LOW);
        sig_ped++;
      }
    }
    
  }
  delay(1000);
}
