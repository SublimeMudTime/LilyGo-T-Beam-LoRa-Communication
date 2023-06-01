# V2V Smart Communication for Emergency Vehicles

![Project Image](project_image.jpg)

## Overview
V2V Smart Communication for Emergency Vehicles is a project aimed at enabling wireless communication and location sharing among emergency vehicles. The system enhances the efficiency and effectiveness of emergency response by allowing vehicles such as ambulances and fire trucks to communicate with each other. It facilitates improved traffic coordination and enhanced safety measures.

The project utilizes LoRa (Long Range) wireless communication technology, which provides long-range coverage with low power consumption. It involves the use of LoRa modules, LoRa antennas, Arduino or similar microcontrollers, GPS modules, and OLED displays.

LoRa modules are responsible for wireless communication between emergency vehicles and a central control point. They offer the advantages of low power consumption and long-range communication capabilities. GPS modules accurately determine the location information of each vehicle, enabling real-time sharing of location data among the vehicles.

An Arduino or similar microcontroller is employed to control the system components. It facilitates the connection of LoRa modules and GPS modules, as well as the processing and display of received data. OLED displays are utilized to provide visual output of important information, allowing vehicles and the control center to read the received data, such as distance information.

## Features
- Wireless communication between emergency vehicles using LoRa technology
- Accurate location tracking and sharing through GPS modules
- Efficient traffic coordination and enhanced safety measures
- Real-time distance calculation and display
- Low power consumption and long-range communication

## Installation
1. Clone this repository.
2. Connect the LoRa modules, GPS modules, Arduino, and OLED displays as per the provided circuit diagram.
3. Upload the receiver code to the Arduino.
4. Upload the transmitter code to each emergency vehicle's Arduino.
5. Power up the devices and ensure they are within LoRa communication range.
6. The OLED displays will show the received distance information.

## Usage
- Ensure that the emergency vehicles are equipped with the necessary hardware components.
- Power on the devices and wait for the LoRa communication to establish.
- The vehicles will start communicating with each other and exchanging location data.
- The OLED displays will show the received distance information.
- Use the information to coordinate traffic and optimize emergency response.

## Acknowledgements
- [TinyGPS++ Library](https://github.com/mikalhart/TinyGPSPlus) - For GPS data parsing.
- [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306) - For OLED display support.
- [LoRa Library](https://github.com/sandeepmistry/arduino-LoRa) - For LoRa communication.

