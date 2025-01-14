#include <MPU6050_tockn.h>
#include <Wire.h>
#include <EEPROM.h>

MPU6050 mpu6050(Wire);

#include <ESP32Servo.h>
  Servo myServo; 
  Servo myServo1;
  const int servoPin = 18;
  const int servoPin1 = 19;
  int goc=90;

#include "BluetoothSerial.h"
String device_name = "ESP32-BT-Slave";
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif
BluetoothSerial SerialBT;
char c;

struct Offsets {
    int16_t AcX;
    int16_t AcY;
    int16_t AcZ;
    int ID;
};
Offsets offsets;

int timecurrent,timepre;
int looptime=10;
int dungthang=1;
float alpha=0.4;
float preGyroX=0;
float gyroX=0;
float angleX=0;
float preangleX=0;
//200, 10, 0.1
float K1=172, K2=4.17,K3=0.00005;
int pwm=0;
float targetAngle = 0;
float integral = 0;
float integralPart;
float derivative;
float error, lastError = 0;
float ANGLE_FIXRATE_1 = 1;
unsigned long dt;
unsigned long timee = 0;
float time_delta;

hw_timer_t *timer = NULL;
volatile bool timerFlag = false;
void IRAM_ATTR onTimer() {
  dc();
}

void setup() {
  Serial.begin(115200);
  pinMode(32,INPUT);
  pinMode(34,INPUT);
  pinMode(35,INPUT);
  pinMode(17,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(2,OUTPUT);
  myServo.attach(servoPin);
  myServo1.attach(servoPin1);
  Wire.setClock(400000);
  Wire.begin();
  mpu6050.begin();
  SerialBT.begin(device_name);
  mpu6050.setGyroOffsets(-0.6,1.51,-0.18);
  timer = timerBegin(200000);
  timerAlarm(timer, 1000, true, 0);
  timerAttachInterrupt(timer, onTimer);
  timerStart(timer);
}

void loop() {
    ble();
  unsigned long now = millis();
  dt = now - timee;
   timee = now;
  time_delta = dt / 1000.0;
  mpu6050.update();
  getdata();
   checkgoc();
    if(dungthang==1)
    {

            if(angleX < targetAngle){ 
            targetAngle += ANGLE_FIXRATE_1 * time_delta;
            }
            else if(angleX > targetAngle){
            targetAngle -= ANGLE_FIXRATE_1 * time_delta;
            }
            error = targetAngle-angleX;
            Serial.print(angleX);
            Serial.print(",");             
            Serial.print(targetAngle);
            Serial.println(",");
             Serial.print(pwm);
            Serial.println(",");
          integral += (error * time_delta); 
          derivative = (error - lastError) / (time_delta);
          pwm = constrain(K1*error+K2*derivative+K3*integralPart,-255,255);
          lastError=error;
    }
    else if(dungthang==0){pwm=0;}
}