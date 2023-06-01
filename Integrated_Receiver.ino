#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND    868E6

TinyGPSPlus gps;
HardwareSerial GPSSerial(1);

void setupGPS() {
  GPSSerial.begin(9600, SERIAL_8N1, 34, 12);
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

  //Serial.println("packet: " + data);
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Wire.begin();
  Wire.setClock(400000);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  LoRa.setPins(SS, RST, DI0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  setupGPS();
}

void loop() {
  while (GPSSerial.available() > 0) {
    gps.encode(GPSSerial.read());
  }

  if (gps.location.isValid()) {
    float latitude = gps.location.lat();
    float longitude = gps.location.lng();

    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      String packet = readData();
      String receivedLatitude = packet.substring(packet.indexOf("LAT:") + 4, packet.indexOf(","));
      String receivedLongitude = packet.substring(packet.indexOf("LON:") + 4);

      float receivedLat = receivedLatitude.toFloat();
      float receivedLon = receivedLongitude.toFloat();

      // Hesaplanan mesafeyi burada gösterebilirsiniz
      float distance = calculateDistance(latitude, longitude, receivedLat, receivedLon);
     
      displayData("Distance: " + String(distance) + " meters");

      Serial.println("****************************************");
      Serial.println("GPS Georaphic Coordinates ");
      Serial.println("----Transmitter Latitude: " + String(receivedLatitude) + " m");
      Serial.println("----Transmitter Longitude: " + String(receivedLongitude) + " m");
      Serial.println("----Receiver Latitude: " + String(latitude) + " m");
      Serial.println("----Receiver Longitude: " + String(longitude) + " m");
      Serial.println("----Distance: " + String(distance) + " m");
      Serial.println("****************************************");
      
    }
  }

  delay(1000);
}
float calculateDistance(float lat1, float lon1, float lat2, float lon2) {
  // havesine fonksiyonu
  float r = 6371000; // Dünya yarıçapı (metre cinsinden)
  float dLat = radians(lat2 - lat1);
  float dLon = radians(lon2 - lon1);
  float a = sin(dLat / 2) * sin(dLat / 2) + cos(radians(lat1)) * cos(radians(lat2)) * sin(dLon / 2) * sin(dLon / 2);
  float c = 2 * atan2(sqrt(a), sqrt(1 - a));
  float distance = r * c;

  
  return distance;
}
// float calculateDistance2(float lat1, float lon1, float lat2, float lon2) {
//   // kartezyen düzlemde 2 nokt arası mesafe hesaplama
//   float distance = sqrt(sq(lat1 - lat2) + sq(lon1 - lon2));
//   return distance;
// }
