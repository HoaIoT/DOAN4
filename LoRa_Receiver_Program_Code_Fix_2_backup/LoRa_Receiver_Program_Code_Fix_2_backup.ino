// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPL6Or_UVh_J"
#define BLYNK_TEMPLATE_NAME "IoT project"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "BlynkEdgent.h"

#define ss 15
#define rst 16
#define dio0 4

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String counter;
String soilMoistureValue;
String soilmoisturepercentreal;
String soiltemp;
String temperature;
String humidity;

String message;
int a ;
int b ;
int c ;
int chedo;
unsigned long t1 = 0 ;


BLYNK_CONNECTED() {
  Blynk.syncVirtual(V6, V7, V8, V9);
}
BLYNK_WRITE(V6){
   a = param.asInt();
   Serial.print(a);
}
BLYNK_WRITE(V7){
   b =param.asInt();
   Serial.print(b);
}
BLYNK_WRITE(V8){
  c =param.asInt();
  Serial.print(c);
}
BLYNK_WRITE(V9){
  chedo =param.asInt();
  Serial.print(chedo);
}


  
void setup() {
  Serial.begin(115200);
  BlynkEdgent.begin();
  while (!Serial);

  Serial.println("LoRa Receiver");

  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  display.clearDisplay();
}

void loop() {
  BlynkEdgent.run();
  // try to parse packet
  int pos1, pos2, pos3, pos4, pos5;
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    // received a packet
    Serial.print("Received packet:  ");
    String LoRaData = LoRa.readString();
    Serial.print(LoRaData);
    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

    pos1 = LoRaData.indexOf('/');
    pos2 = LoRaData.indexOf('&');
    pos3 = LoRaData.indexOf('#');
    pos4 = LoRaData.indexOf('@');
    pos5 = LoRaData.indexOf('$');

    counter = LoRaData.substring(0, pos1);
    soilMoistureValue = LoRaData.substring(pos1 + 1, pos2);
    soilmoisturepercentreal = LoRaData.substring(pos2 + 1, pos3);
    soiltemp = LoRaData.substring(pos3 + 1, pos4);
    temperature = LoRaData.substring(pos4 + 1, pos5);
    humidity = LoRaData.substring(pos5 + 1, LoRaData.length());

    //send data to blynk
    Blynk.virtualWrite(V1, soilmoisturepercentreal); //Soil Moisture
    Blynk.virtualWrite(V2, soiltemp); //Soil Temperature
    Blynk.virtualWrite(V3, temperature);  // for Temperature
    Blynk.virtualWrite(V4, humidity);  //for Humidity

    Serial.print(F("Packet No = "));
    Serial.println(counter);

    Serial.print(F("Soil Moisture: "));
    Serial.print(soilmoisturepercentreal);
    Serial.println(F("%"));

    Serial.print(F("Soil Temperature: "));
    Serial.print(soiltemp);
    Serial.println(F("°C"));

    Serial.print(F("Temperature: "));
    Serial.print(temperature);
    Serial.println(F("°C"));

    Serial.print(F("Humidity = "));
    Serial.print(humidity);
    Serial.println(F("%"));

    Serial.print("Soil Moisture Value: ");
    Serial.print(soilMoistureValue);

    Serial.println();

    if (soilmoisturepercentreal.toInt() > 100)
    {
      display.clearDisplay();

      // display Soil Humidity
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.setCursor(0, 5);
      display.print("Do Am Dat: ");
      display.print("100");
      display.print(" %");

//      // display soil temperature
//      display.setCursor(0, 20);
//      display.print("Soil Temp: ");
//      display.print(soiltemp);
//      display.print(" ");
//      display.cp437(true);
//      display.write(167);
//      display.print("C");

      // display air temperature
      display.setCursor(0, 35);
      display.print("Nhiet Do: ");
      display.print(temperature);
      display.print(" ");
      display.cp437(true);
      display.write(167);
      display.print("C");

      // display relative humidity of Air
      display.setCursor(0, 50);
      display.print("Do Am Dat: ");
      display.print(humidity);
      display.print(" %");

      display.display();
      delay(1500);
    }
    else if (soilmoisturepercentreal.toInt() < 0)
    {
      display.clearDisplay();

      // display Soil Humidity
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.setCursor(0, 5);
      display.print("Đo Am Dat: ");
      display.print("0");
      display.print(" %");

//      // display soil temperature
//      display.setCursor(0, 20);
//      display.print("Soil Temp: ");
//      display.print(soiltemp);
//      display.print(" ");
//      display.cp437(true);
//      display.write(167);
//      display.print("C");

      // display air temperature
      display.setCursor(0, 35);
      display.print("Nhiet Do: ");
      display.print(temperature);
      display.print(" ");
      display.cp437(true);
      display.write(167);
      display.print("C");

      // display relative humidity of Air
      display.setCursor(0, 50);
      display.print("Do Am: ");
      display.print(humidity);
      display.print(" %");

      display.display();
      delay(1500);
    }
    else if (soilmoisturepercentreal.toInt() >= 0 && soilmoisturepercentreal.toInt() <= 100)
    {
      display.clearDisplay();

      // display Soil humidity
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.setCursor(0, 5);
      display.print("Do Am Dat: ");
      display.print(soilmoisturepercentreal);
      display.print(" %");

//      // display soil temperature
//      display.setCursor(0, 20);
//      display.print("Soil Temp: ");
//      display.print(soiltemp);
//      display.print(" ");
//      display.cp437(true);
//      display.write(167);
//      display.print("C");

      // display air temperature
      display.setCursor(0, 35);
      display.print("Nhiet do: ");
      display.print(temperature);
      display.print(" ");
      display.cp437(true);
      display.write(167);
      display.print("C");

      // display relative humidity of Air
      display.setCursor(0, 50);
      display.print("Do am: ");
      display.print(humidity);
      display.print(" %");

      display.display();
      delay(1500);
    }
    if (soilmoisturepercentreal.toInt() >= 0 && soilmoisturepercentreal.toInt() <= 30)
    {
      Serial.println("needs water, send notification");
      //send notification
      Blynk.logEvent("water_your_plants", "Please Water your plants they are about to die...") ;
      Serial.println("Motor is ON");
      Blynk.virtualWrite(V5, 255);
      delay(1000);
    }
    else if (soilmoisturepercentreal.toInt() > 30 && soilmoisturepercentreal.toInt() <= 100)
    {
      Serial.println("Soil Moisture level looks good...");
      Serial.println("Motor is OFF");
      Blynk.virtualWrite(V5, 0);
      delay(1000);
    }

  }
  
  if(millis() - t1 > 2000){
  message = String(a) +"|" + String(b) + "!" + String(c) + "*" + String(chedo);
  LoRa.beginPacket();
  LoRa.print(message);
  Serial.println(message);
  LoRa.endPacket();
  t1 = millis();
  }
}
