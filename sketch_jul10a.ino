#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>

#include <BlynkSimpleShieldEsp8266.h>
#include <Servo.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

//#include <SoftwareSerial.h>

char auth[] = "Auth Key";
char ssid[] = "Nama SSID";
char pass[] = "Password";

#define EspSerial Serial

WidgetTerminal terminal(V2);
BlynkTimer timer;
WidgetRTC rtc;
Servo servo;
const int inputPin = 5;
//int val = 0;
const int led = 13;
const int relay = 4;
//int relayState = digitalRead(relay);
int x;
//char currentTime[9];
//String c = String(currentTime);
//return c;


//SoftwareSerial EspSerial(0, 1);

//#define ESP8266_BAUD 115200

ESP8266 wifi(&EspSerial);


void setup() {
  Serial.begin(115200); //set console baud rate 
  delay(10);
  
  EspSerial.begin(115200); //set esp baud rate
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass); //auth, wifi, ssid, pass
  Serial.begin(9600);
  pinMode(relay, OUTPUT);
  //pinMode(relay, INPUT);
  pinMode(inputPin, INPUT);
  pinMode(led, OUTPUT);
  digitalWrite(relay, 1); //slot atau doorlock dalam keadaan kekunci
  servo.attach(8);
  servo.write(180);
  rtc.begin();
  setSyncInterval(1);
  //timer.setInterval(1000L, clockDisplay);
}

void loop() {
  Blynk.run();
  timer.run();
  int pir = digitalRead(inputPin);
  if (pir == HIGH && x == 0){ // jika pir mendeteksi gerakan dan user tidak menekan button
    Blynk.notify("Ada Penyusup!!!");
    delay(100);
  }

  if (pir == LOW && x == 1) { // jika pir tidak mendeteksi gerakan dan button user bernilai 1 maka pintu otomatis tertutup (masih ada masalah)
    Blynk.virtualWrite(V2, "\n Pintu Tertutup")  ;
    Blynk.virtualWrite(V0, 0);
    //digitalWrite(relay, HIGH);
    for (pos = 90; pos < 180; pos+=1){
    servo.write(180);
    delay(15);
    }
    delay(1000);
    for (pos = 180; pos >=1; pos-=1) {
      servo.write(pos);
      delay(15);
    }
}
}

BLYNK_WRITE(V0){
  String cTime = String(hour()) + ":" + minute() + ":" + second();
  
  x = param.asInt(); // variabel button pada blynk
  //int buka = param.asInt();
  int val = digitalRead(inputPin);
    if (val == HIGH && x == 1) { // jika pir mendeteksi gerakan dan user menekan button buka maka pintu terbuka
    Blynk.virtualWrite(V2, "\n Pintu Terbuka " + cTime);
    digitalWrite(relay, LOW);
    servo.write(90);
    delay(100);  
    } 
        
  if (x == 1) { // buka pintu
    Blynk.virtualWrite(V2, "\n Pintu Terbuka "+ cTime);
    digitalWrite(relay, HIGH);
    servo.write(90);
    } else if (x == 0) { //tutup pintu
    Blynk.virtualWrite(V2, "\n Pintu Tertutup " + cTime);
    digitalWrite(relay, LOW);
    servo.write(180);
    }
}
