#include <MPU6500_WE.h>
#include <Wire.h>
#define MPU6500_ADDR 0x68
#define BUZZER_PIN 4
#define BUTTON_PIN 18

// unsigned long is used for extremely positive big numbers
unsigned long lastTriggerTime = 0;
int cooldown = 1000;

MPU6500_WE myMPU6500 = MPU6500_WE(MPU6500_ADDR);

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if(!myMPU6500.init()){
    Serial.println("MPU6500 does not respond");
  }
  else{
    Serial.println("MPU6500 is connected");
  }
}

void loop() {
    // speed of spinning
    xyzFloat gyr = myMPU6500.getGyrValues();
    // the force of your wrist
    xyzFloat acc = myMPU6500.getGValues();

    float maxVal = max(abs(gyr.x), max(abs(gyr.y), abs(gyr.z)));
    float maxAcc = myMPU6500.getResultantG(acc);
    //records how many ms has past since the beginning that ESP32 is opened
    unsigned long now = millis(); 

    if (maxVal > 100 || maxAcc > 1.5){
        Serial.print("Spin angle(deg/s) X:"); Serial.print(gyr.x);
        Serial.print("X:"); Serial.print(gyr.x);
        Serial.print("Y:"); Serial.print(gyr.y);
        Serial.print("Z:"); Serial.print(gyr.z);
        Serial.print("acceleration(g) X:"); Serial.print(acc.x);
        Serial.print(" Y:"); Serial.print(acc.y);
        Serial.print(" Z:"); Serial.print(acc.z);
        Serial.println("---");
    }

    delay(50);

    if (abs(gyr.z) > 200 && maxAcc > 1.8 && (now - lastTriggerTime > cooldown)) {
      // Serial is the communicating channel between the computer and ESP32
      Serial.print("FIRE");
      for(int i = 0; i < 10; i++) {
        tone(BUZZER_PIN, 200);
        delay(20);
        noTone(BUZZER_PIN);
        delay(20);
      }
      lastTriggerTime = now;
    }
}
