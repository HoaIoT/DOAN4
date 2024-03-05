#include <Wire.h>
#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
 
#define DHTPIN 5          //pin where the dht22 is connected
DHT dht(DHTPIN, DHT22);
#define relay 8
#define relay_1 7
#define relay_2 4
#define ss 10
#define rst 9
#define dio0 2
#define ONE_WIRE_BUS 6
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

String LoRaMessage = "";
String receivedMessage = "";
int counter = 0;
const int AirValue = 0;   //you need to replace this value with Value_1
const int WaterValue = 1023;  //you need to replace this value with Value_2
const int SensorPin = A0;
int soilMoistureValue = 0;
int soilmoisturepercent = 0;
unsigned long t1 = 0 ;
String QUAT;
String DEN;
String MOTOR;
String CHEDO;

void setup() 
{
  Serial.begin(115200);
  dht.begin();
  sensors.begin(); // Dallas temperature
  pinMode(relay, OUTPUT);
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  while (!Serial);
  Serial.println("LoRa Sender");
  LoRa.setPins(ss, rst, dio0);
    if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    delay(100);
    while (1);
  }
}
 
void loop() 
{
  soilMoistureValue = analogRead(SensorPin);  //put Sensor insert into soil
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 1, 100);
int soilmoisturepercentreal = 100 - soilmoisturepercent;
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);
 
  if (isnan(h) || isnan(t)) 
  {
  Serial.println("Failed to read from DHT sensor!");
  //return;
  }
//  Serial.print("Soil Moisture Value: ");
//  Serial.println(soilMoistureValue);
//  
//  Serial.print("Soil Moisture: ");
//  Serial.print(soilmoisturepercentreal);
//  Serial.println("%");
//  
//  Serial.print("Soil Temperature: ");
//  Serial.print(temp);
//  Serial.println("°C");
//  
//  Serial.print("Temperature: ");
//  Serial.print(t);
//  Serial.println("°C");
//  
//  Serial.print("Humidity: ");
//  Serial.print(h);
//  Serial.println("%");
//  Serial.println("");
//  
//  Serial.print("Sending packet: ");
//  Serial.println(counter);
  

//*********************************nhandulieudieukhientubotrungtam**************************
 // received data
 int pos6, pos7,pos8; 
   int packetSize = LoRa.parsePacket();
   Serial.println(packetSize);
  if (packetSize) {
    receivedMessage = "";
    String receivedMessage = LoRa.readString();
    while (LoRa.available()) {
      receivedMessage += (char)LoRa.read();
    }
    Serial.print("Received packet: ");
    Serial.println(receivedMessage);
    
    pos6 = receivedMessage.indexOf('|');
    pos7 = receivedMessage.indexOf('!');
    pos8 = receivedMessage.indexOf('*');

    QUAT = receivedMessage.substring(0, pos6);
    DEN = receivedMessage.substring(pos6 + 1, pos7);
    MOTOR = receivedMessage.substring(pos7 + 1, pos8);
    CHEDO = receivedMessage.substring(pos8 + 1, receivedMessage.length());

    Serial.println(QUAT);
    Serial.println(DEN);
    Serial.println(MOTOR);
    Serial.println(CHEDO);
  //xử lý dữ liệu
  if(CHEDO == "1"){
  if(QUAT == "1"){
    digitalWrite(relay,HIGH);
    Serial.println("QUAT ON");
  }
  else if(QUAT == "0"){
    digitalWrite(relay,LOW);
    Serial.println("QUAT OFF");
  }
  if(DEN == "1"){
    digitalWrite(relay_1,HIGH);
    Serial.println("QUAT ON");
  }
  else if(DEN == "0"){
    digitalWrite(relay_1,LOW);
    Serial.println("QUAT OFF");
  }
    if(MOTOR == "1"){
    digitalWrite(relay_2,HIGH);
    Serial.println("QUAT ON");
  }
  else if(MOTOR == "0"){
    digitalWrite(relay_2,LOW);
    Serial.println("QUAT OFF");
  }
  
  }
  else if(CHEDO == "0"){
    if (soilmoisturepercentreal >= 0 && soilmoisturepercentreal <= 30)
  {
    Serial.println("Plants need water..., notification sent");
    digitalWrite(relay_2,HIGH);
    Serial.println("Motor is ON");
    
     }
  else if (soilmoisturepercentreal > 35 && soilmoisturepercentreal <= 100)
  {
    Serial.println("Soil Moisture level looks good...");
    digitalWrite(relay_2,LOW);
    Serial.println("Motor is OFF");
    
  }
  if (t >= 30 )
  {
    Serial.println("Plants need water..., notification sent");
    digitalWrite(relay,HIGH);
    digitalWrite(relay_1,LOW);
    Serial.println("QUAT is ON");

     }
  else if(t > 26 && t < 30){
        digitalWrite(relay,LOW);
        digitalWrite(relay_1,LOW);
  }
  else if (t <= 26)
  {
    Serial.println("Soil Moisture level looks good...");
    digitalWrite(relay_1,HIGH);
    digitalWrite(relay,LOW);
    Serial.println("DEN is ON");
    
  }

  
  }
  
 
}




  
//*******************************guidulieudenbotrungtam********************************

  if(millis() - t1 > 1500){
   t1 = millis();
    LoRaMessage = String(counter) +"/" + String(soilMoistureValue) + "&" + String(soilmoisturepercentreal)
                + "#" + String(temp) + "@" + String(t) + "$" + String(h);
  
   // send packet

  LoRa.beginPacket();
  LoRa.print(LoRaMessage);
  Serial.println(LoRaMessage);
  LoRa.endPacket();
  counter++;
 
  }

}
