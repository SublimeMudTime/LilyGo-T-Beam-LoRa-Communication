#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND    868E6

unsigned int counter = 0;
char rssi[9] = "RSSI --";
char packSize[3] = "--";
char packet[64];

void setup() {
  pinMode(2, OUTPUT);

  Serial.begin(9600);
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Sender Test");

  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  delay(1000);
}

void loop() {
  counter++;
  Serial.println(String(counter));

  LoRa.beginPacket();
  sprintf(packet, "packet %d", counter);
  LoRa.print(packet);
  LoRa.endPacket();

  delay(1000);
}
