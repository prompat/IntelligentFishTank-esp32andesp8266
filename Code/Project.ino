#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

#define TRIGGER D1
#define ECHO    D2   
//#define Max_Distance 200

Servo petGate;

//ตัวแปร servo
int gateStart = 10;//45
int gateEnd =55;
int gateOpenTime = 180;

char auth[] = "FZgmHpeHV29vGceyM6ytQOtGM62LXNyp";
char ssid[] = "Guy";
char pass[] = "1212312121";

const int analogInPin = A0;
int sensorValue = 0;
unsigned long int avgValue;
float b;
int buf[10], temp;

BLYNK_WRITE(V0)//Feed
{
     petGate.write(gateEnd);
     delay(gateOpenTime);
     petGate.write(gateStart);
     Serial.println("Success");
     Blynk.notify("การให้อาหารสำเร็จ"); 
}

BLYNK_WRITE(V1)
{
  Serial.print("Got a value: ");
  Serial.println(param.asInt());
  
  if(param.asInt() == 0)
  {
     petGate.write(gateStart);
  }
  if(param.asInt() == 1)
  {
    for(int i=0;i<2;i++)
      {
        petGate.write(gateEnd);
        delay(gateOpenTime);
        petGate.write(gateStart);
        delay(gateOpenTime);
      }
  }
  Blynk.notify("การให้อาหารตามเวลาที่ตั้งไว้สำเร็จ");
  Blynk.email("guyynwa@gmail.com", "การแจ้งเตือนจากตู้ปลา", "การให้อาหารตามเวลาที่ตั้งไว้สำเร็จ!!!");
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  petGate.attach(2);
  petGate.write(gateStart);
  // กำหนเค่าเริ่มต้น Blynk
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  //ตัวแปร PH Module
  for (int i = 0; i < 10; i++) {
  buf[i] = analogRead(analogInPin);
  delay(10);
  }
    for (int i = 0; i < 9; i++)
    {
      for (int j = i + 1; j < 10; j++)
      {
      if (buf[i] > buf[j])
      {
      temp = buf[i];
      buf[i] = buf[j];
      buf[j] = temp;
      }
    }
  }
  avgValue = 0;
  for (int i = 2; i < 8; i++)
  avgValue += buf[i];
  float pHVol = (float)avgValue * 5.0 / 1024 / 6;
  
  float phValue=3.5*pHVol;  //convert the millivolt into pH value
  phValue=phValue-4;
  
//  float phValue = -5.70 * pHVol + 21.34;

  if((phValue >= 10)&&(phValue <= 14)){
  
      Blynk.notify("กรุณาเปลี่ยนน้ำในตู้ปลา!!!");
      Blynk.email("guyynwa@gmail.com", "การแจ้งเตือนจากตู้ปลา", "กรุณาเปลี่ยนน้ำในตู้ปลา!!!");
      Serial.print("sensor = ");
      Serial.println(phValue);
      Blynk.virtualWrite(V2,phValue);
      }
       if((phValue <= 5)&&(phValue >= 0)){
  
      Blynk.notify("กรุณาเปลี่ยนน้ำในตู้ปลา!!!");
      Blynk.email("guyynwa@gmail.com", "การแจ้งเตือนจากตู้ปลา", "กรุณาเปลี่ยนน้ำในตู้ปลา!!!");
      Serial.print("sensor = ");
      Serial.println(phValue);
      Blynk.virtualWrite(V2,phValue);
      }
  else{
      Serial.print("sensor = ");
      Serial.println(phValue);
      Blynk.virtualWrite(V2,phValue);
      delay(1000);
      Blynk.run();
  }
  //ตัวแปร ultra sonic
   
  long duration, distance;
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;
  if((distance >= 9)&&(distance <= 15)){

    Blynk.notify("อาหารปลาใกล้หมด!!!");
    Blynk.email("guyynwa@gmail.com", "การแจ้งเตือนจากตู้ปลา", "อาหารปลาใกล้หมด!!!");
    Serial.print(distance);
    Serial.println("Centimeter:");
    Blynk.virtualWrite(V5, distance);
    }
  else{
    Serial.print(distance);
    Serial.println("Centimeter:");
    Blynk.virtualWrite(V5, distance);
    delay(1000);
    Blynk.run();
  }
} 
