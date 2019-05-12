# Arduino_cameraVC0706
Get environment data with camera and other sensors, store them in SD card and cover regularly.
## Project function
This project is like an IoT system, but it's local, store all data in SD card.

Sensors can get environment temperature, humidity, and infrared. If environment conditions can't feet the requirements, the buzzer will set the alarm. Plus camera can take pictures after every certain delay, and all data will be stored in SD card. Environment temperature, humidity and infrared will be write in a txt, and pictures will be named like "IMAGE000.jpg". All data in SD card can be covered after certain times of storage.
## Pre-requirements
Add lib to your libraries and run final.ino.
## Hardware connection
Chose arduino mega as mian board, DHT22 as temperature and humidity sensor, camera VC0706 as camera, a infrared sensor and a buzzer to set alarm.

DHT22      --->   Pin 7

Buzzer     --->   Pin 8

Infrared   --->   Pin A0

Camera Tx  --->   Pin A15

Camera Rx  --->   Pin 3

SD CS      --->   Pin 53

SD SCK     --->   Pin 52

SD MOSI    --->   Pin 51

SD MISO    --->   Pin 50
## How to use serial port debugging tools for camera VC0706
