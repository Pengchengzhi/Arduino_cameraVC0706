# Arduino_cameraVC0706
Description: Collect environment data with camera and other sensors, store them in SD card and cover regularly.
## Project function
This project is like an IoT system, but locally distributed, store all data in SD card.

Sensors can collect environment temperature, humidity, and infrared. If environment conditions can't meet the requirements we set, turn on the buzzer. CamerVC0706 can take pictures repeatingly after delay, and all data will be stored in SD card. Environment temperature, humidity and infrared will be write in a `.txt` file, and pictures will be named like `IMAGE000.jpg`. Those data in SD card can be covered after certain times of storage.
## Pre-requirements
Add libs to your libraries and run `final.ino`.
## Hardware connection
* Chose arduino mega as mian board, DHT22 as temperature and humidity sensor, camera VC0706 as camera, a infrared sensor and a buzzer to set alarm.

| Sensors   | Arduino  |
| :-----:   | :-----:  |
|  DHT22    |  Pin 7   |
|  Buzzer   |  Pin 8   |
| Infrared  |  Pin A0  |
| Camera Tx |  Pin A15 |
| Camera Rx |  Pin 3   |
|   SD CS   |  Pin 53  |
|   SD SCK  |  Pin 52  |
|  SD MOSI  |  Pin 51  |
|  SD MISO  |  Pin 50  |
## How to use serial port debugging tools for camera VC0706
* The following is for `VC0706CommTool`, not for `XCOM`.

Use a USB to serial port converter, connect `camera Tx` to `converter Rx`, `camera Rx` to `converter Tx`.

And chose the correct serial port, then

`“Open” ---> Fbuf Ctrl ---> Stop CFbuf ---> Sel File ----> Read`

And you can see the pictures.
