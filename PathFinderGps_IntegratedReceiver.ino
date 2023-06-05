#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <vector>

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

struct Point {
  float latitude;
  float longitude;
};

std::vector<Point> transmitterPath;
std::vector<Point> receiverPath;

float decreaseThreshold = 5.0;  // Azalma eşik değeri (örneğin 5 metre)
float increaseThreshold = 5.0;  // Artış eşik değeri (örneğin 5 metre)
float previousDistance = 0.0;

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

void displayDistance(float distance) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  if (distance >= increaseThreshold) {
    display.println("Distance increased!");
    display.println("Stay on the Line.");
    display.setTextSize(1);
    display.println("Distance: Increasing");
  } else if (distance <= -decreaseThreshold) {
    display.println("Distance decreased!");
    display.println("Stand Your Position!");
    display.setTextSize(2);
    display.println("CAREFUL!");
    display.setTextSize(1);
    display.println("Distance: Decreasing");  
  } else {
    display.println("Good Driving,");
    display.setTextSize(2);
    display.println("Drive Safely..");
    display.setTextSize(1);
    display.println("Distance: Stable");
  }

  display.display();
}

void displaySerialDistance(float distance) {
  if (distance >= increaseThreshold) {
    Serial.println("Distance increased! Drive safely.");
  } else if (distance <= -decreaseThreshold) {
    Serial.println("Distance decreased! Pay attention.");
  } else {
    Serial.println("Distance is stable. Drive carefully.");
  }
}

float calculateDistance(float lat1, float lon1, float lat2, float lon2) {
  // Haversine formülü ile mesafe hesaplama
  float r = 6371000; // Dünya yarıçapı (metre cinsinden)
  float dLat = radians(lat2 - lat1);
  float dLon = radians(lon2 - lon1);
  float a = sin(dLat / 2) * sin(dLat / 2) + cos(radians(lat1)) * cos(radians(lat2)) * sin(dLon / 2) * sin(dLon / 2);
  float c = 2 * atan2(sqrt(a), sqrt(1 - a));
  float distance = r * c;

  return distance;
}

bool checkIntersection(const std::vector<Point>& path1, const std::vector<Point>& path2) {
  // Yolların kesişme kontrolünü gerçekleştirme
  // Eğer yollar kesişiyorsa, true döndür
  // Eğer yollar kesişmiyorsa, false döndür
  // İki yolu kesişme algoritmasına göre kontrol et
}

void displayWarningMessage(const String& message) {
  Serial.println(message);
  // Uyarı mesajını ekrana yazdırma işlemleri
}

void determinePaths() {
  // Transmitter ve receiver yollarını belirleme işlemleri
  // Örnek olarak sabit yollar tanımlanabilir veya GPS verileri kullanılabilir
  // İşlemler sonucunda transmitterPath ve receiverPath vektörleri güncellenmelidir
  // Örnek olarak:
  
  transmitterPath.clear();
  receiverPath.clear();
  
  // Transmitter yolunu tanımla
  Point transmitterPoint1;
  transmitterPoint1.latitude = 40.12345;
  transmitterPoint1.longitude = 29.67890;
  
  Point transmitterPoint2;
  transmitterPoint2.latitude = 40.23456;
  transmitterPoint2.longitude = 29.78901;
  
  transmitterPath.push_back(transmitterPoint1);
  transmitterPath.push_back(transmitterPoint2);
  
  // Receiver yolunu tanımla
  Point receiverPoint1;
  receiverPoint1.latitude = 40.34567;
  receiverPoint1.longitude = 29.89012;
  
  Point receiverPoint2;
  receiverPoint2.latitude = 40.45678;
  receiverPoint2.longitude = 29.90123;
  
  receiverPath.push_back(receiverPoint1);
  receiverPath.push_back(receiverPoint2);
}

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (true);
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);

  setupGPS();
  determinePaths();
}

void loop() {
  // GPS verilerini oku
  while (GPSSerial.available() > 0) {
    gps.encode(GPSSerial.read());
  }

  if (gps.location.isValid()) {
    float latitude = gps.location.lat();
    float longitude = gps.location.lng();

    // En yakın noktalardan mesafeyi hesapla
    float transmitterDistance = calculateDistance(latitude, longitude, transmitterPath[0].latitude, transmitterPath[0].longitude);
    float receiverDistance = calculateDistance(latitude, longitude, receiverPath[0].latitude, receiverPath[0].longitude);

    // Yön bilgisini belirle
    float distance = receiverDistance - transmitterDistance;
  
    // Mesafeyi ekrana yazdır
    displayDistance(distance);

    // Mesafeyi seri port üzerinden yazdır
    displaySerialDistance(distance);

    // Yolların kesişme kontrolünü yap
    if (checkIntersection(transmitterPath, receiverPath)) {
      displayWarningMessage("Yollar kesişecek!");
    } else {
      displayWarningMessage("Yollar kesişmeyecek.");
    }

    // Önceki mesafeyi güncelle
    previousDistance = distance;
  }
}
