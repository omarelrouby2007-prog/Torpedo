

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
WiFiUDP udp;

const char *ssid = "YOUR_WIFI_NAME";       
const char *password = "YOUR_WIFI_PASSWORD";
const int ENA=25;
const int IN1=26;
const int IN2=27;
const int ENB=32;
const int IN3=33;
const int IN4=13;
const int SDA_PIN=21;
const int SCL_PIN=22;
const int ENA2=4;
const int IN5=16;
const int IN6=17;
const int ENB2=18;
const int IN7=19;
const int IN8=23;
const uint16_t PORT = 8080;
const uint8_t MPU_ADDR=0*68;
IPAddress labtopIp;
uint16_t labtopPort=0;
unsigned long lastTelemetry=0;
unsigned long lastCmd = 0;



void setup() {
  Serial.begin(115200);


    pinMode(IN1,OUTPUT);
    pinMode(IN2,OUTPUT);
    pinMode(IN3,OUTPUT);
    pinMode(IN4,OUTPUT);
    pinMode(IN5,OUTPUT);
    pinMode(IN6,OUTPUT);
    pinMode(IN7,OUTPUT);
    pinMode(IN8,OUTPUT);
    ledcAttach(ENA,1000,8);
    ledcAttach(ENB,1000,8);
    ledcAttach(ENA2,1000,8);
    ledcAttach(ENB2,1000,8);
    udp.begin(PORT);
    Wire.begin(SDA_PIN,SCL_PIN);
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);

  Serial.println();
  Serial.println("******************************************************");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void setMotor(int enPin,int inA,int inB,int speed){


               speed=constrain(speed,-255,255);
               if(speed>0){
                digitalWrite(inA,HIGH);
                digitalWrite(inB,LOW);
               }
               else if(speed<0){

                      digitalWrite(inA,LOW);
                      digitalWrite(inB,HIGH);

               }
               else{
                digitalWrite(inA,LOW);
                digitalWrite(inB,LOW);
               }
              ledcWrite(enPin,abs(speed));


}
bool readMPU(int16_t v[7]){

     Wire.beginTransmission(MPU_ADDR);
     Wire.write(0x3B);
     if(Wire.endTransmission(false)!=0)
     return false;
     if (Wire.requestFrom(MPU_ADDR, (uint8_t)14, (uint8_t)true) != 14) return false;
     for(int i=0;i<7;i++){
        int16_t high=Wire.read();
        int16_t low=Wire.read();
        v[i]=(high<<8)|low;


     }
     return true;






}

void readResponse(NetworkClient *client) {
  unsigned long timeout = millis();
  while (client->available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client->stop();
      return;
    }
  }

  
  while (client->available()) {
    String line = client->readStringUntil('\r');
    Serial.print(line);
  }

  Serial.printf("\nClosing connection\n\n");
}
char buf[64];
unsigned long cmd=0;

void loop() {

    int n=udp.parsePacket();
    if(n>0){

      int len=udp.read(buf,sizeof(buf) -1);
      labtopIp=udp.remoteIP();
      labtopPort=udp.remotePort();
      buf[len]='\0';
      int right ,left;
      if(sscanf(buf,"M,%d,%d",&left,&right)==2){


           setMotor(ENA,IN1,IN2,left);
           setMotor(ENB,IN3,IN4,right);
           setMotor(ENA2,IN5,IN6,left);
           setMotor(ENB2,IN7,IN8,right);
           lastCmd=millis();

      }
    }
    if(millis()-lastCmd>500){
      setMotor(ENA,IN1,IN2,0);
      setMotor(ENB,IN3,IN4,0);
      setMotor(ENA2,IN5,IN6,0);
      setMotor(ENB2,IN7,IN8,0);
    }


    if (labtopPort != 0 && millis() - lastTelemetry > 100) {
  lastTelemetry = millis();
  int16_t v[7];
  if (readMPU(v)) {
    udp.beginPacket(labtopIp, labtopPort);
    udp.printf("T,%d,%d,%d,%d,%d,%d", v[0], v[1], v[2], v[4], v[5], v[6]);
    udp.endPacket();
  }
}














    }
     
  

