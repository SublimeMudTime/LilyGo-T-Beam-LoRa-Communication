#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define GPS_SERIAL_NUM 1
#define GPS_RX_PIN 34
#define GPS_TX_PIN 12

TinyGPSPlus gps;
HardwareSerial GPSSerial(GPS_SERIAL_NUM);

void setup() {
  Serial.begin(115200);
  GPSSerial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

  Serial.println("GPS Location:");
}

void loop() {
  while (GPSSerial.available() > 0) {
    gps.encode(GPSSerial.read());
  }

  if (gps.location.isValid()) {
    Serial.print("Latitude  : ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude : ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude  : ");
    Serial.println(gps.altitude.meters());
    Serial.println("--------------------");
  }

  delay(1000);
}
