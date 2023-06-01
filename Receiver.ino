#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND    868E6

void setupI2CDevices() {

  Wire.begin();
  Wire.setClock(400000);

  byte address;
  byte foundAddresses = 0;
  
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();
    
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      
      if (address < 16) {
        Serial.print("0");
      }
      
      Serial.println(address, HEX);
      foundAddresses++;
    }
  }
  
  if (foundAddresses == 0) {
    Serial.println("No I2C devices found");
  }

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void setupLoRaDevices()
{
  LoRa.setPins(SS, RST, DI0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}


String readData() {
  String packet;
  
  while (LoRa.available()) {
    packet = LoRa.readString();
  }
  
  return packet;
}

void displayData(const String& data) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(data);
  display.display();
  
  Serial.println("packet: " + data);
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  setupI2CDevices();
  setupLoRaDevices();
  
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String packet = readData();
    displayData(packet);
    
  }
}
