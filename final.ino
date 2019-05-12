#include "Adafruit_VC0706.h"
#include <dht.h>
#include <SD.h>
#include <SPI.h>
#include <SoftwareSerial.h>
dht DHT;
#define DHT22_PIN 7
#define DelayTime 100000 //count time to take pictures
const int chipSelect = 53;
int buzzer=8;
SoftwareSerial cameraconnection = SoftwareSerial(69, 3);
Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);
static long timer; 
byte Wcount=0; //count time to print something while transforming image
int timecount=0; //count number of images in SD card
int writetxt=0; //wait till I take picture
void setup()
{
    Serial.begin(115200);
    Serial.println("VC0706 Camera test");
    // Try to locate the camera
    if (cam.begin())
    {
        Serial.println("Camera Found:");
    }
    else
    {
        Serial.println("No camera found?");
    }
    // Print out the camera version information (optional)
    char *reply = cam.getVersion();
    if (reply == 0)
    {
        Serial.print("Failed to get version");
    }
    else
    {
        Serial.println("-----------------");
        Serial.print(reply);
        Serial.println("-----------------");
    }
    // set picture size -  640x480, 320x240 or 160x120   
    //cam.setImageSize(VC0706_640x480);        // biggest
    cam.setImageSize(VC0706_320x240);          // medium
    //cam.setImageSize(VC0706_160x120);        // small

    // You can read the size back from the camera (useful)
    uint8_t imgsize = cam.getImageSize();
    Serial.print("Image size: ");
    if (imgsize == VC0706_640x480) Serial.println("640x480");
    if (imgsize == VC0706_320x240) Serial.println("320x240");
    if (imgsize == VC0706_160x120) Serial.println("160x120");
    timer = 0;
    Serial.println("Initializing SD card...");
    pinMode(53, OUTPUT);
    pinMode(A0,INPUT);
    pinMode(buzzer,OUTPUT);
    if (!SD.begin(chipSelect)) 
    {
        //Card failed, or not present
        Serial.println("Card failed, or not present");
        //if failed, check if you forget to insert SD card
    }
    else
    {
        Serial.println("card initialized.");  //if successful print card initialized.
    }
        Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
}

void loop()
{
    Serial.print("DHT22, \t");
    int chk = DHT.read22(DHT22_PIN);  //read data
    switch (chk)
    {
        case DHTLIB_OK:  
            Serial.print("OK,\t"); 
            break;
        case DHTLIB_ERROR_CHECKSUM: 
            Serial.print("Checksum error,\t"); 
            break;
        case DHTLIB_ERROR_TIMEOUT: 
            Serial.print("Time out error,\t"); 
            break;
        default: 
            Serial.print("Unknown error,\t"); 
            break;
    }
    
    // if condition does not fit, set alarm
    // apparently you can change to your numbers
    if(DHT.humidity>60||DHT.humidity<40||DHT.temperature>29||DHT.temperature<19||analogRead(A0)<500)
    {
        digitalWrite(buzzer,HIGH);
    }
    else
    {
        digitalWrite(buzzer,LOW);
    }
    
    //show humidity and temperature
    Serial.print(DHT.humidity, 1);
    Serial.print(",\t");
    Serial.print(DHT.temperature, 1);
    Serial.print(",\t");
    Serial.println(analogRead(A0));
    
    // check if it's time to write data to SD card
    if (writetxt ==1)
    {
        File dataFile = SD.open("Data.txt", FILE_WRITE);
        if (dataFile) 
        {
            dataFile.println("Humidity (%),\tTemperature (C),\tFlame sensor data");
            dataFile.print(DHT.humidity, 1);
            dataFile.print(",\t\t");
            dataFile.print(DHT.temperature, 1);
            dataFile.print(",\t\t");
            dataFile.println(analogRead(A0));
            dataFile.close();
        }
        else 
        {
            Serial.println("error opening Data.txt");
        }
        writetxt=0;
    }
    
    // take pictures
    if (millis() > timer + DelayTime) 
    {
        timer = millis();
        Serial.println("Delay Done!");   
        delay(7000); //optional
        if (! cam.takePicture()) 
            Serial.println("Failed to snap!");
        else 
            Serial.println("Picture taken!");
        delay(5000); //optional
        char filename[13];
        strcpy(filename, "IMAGE000.JPG");
        for (int i = 0; i < 1000; i++) 
        {
            filename[5] = '0' + i/100;
            filename[6] = '0' + i%100/10;
            filename[7] = '0' + i%10;
            // create if does not exist, do not open existing, write, sync after write
            if (! SD.exists(filename)) 
            {
                break;
            }
        }
        // now changed to a new filename
        File imgFile = SD.open(filename, FILE_WRITE);
        delay(2000); //optional
        uint16_t jpglen = cam.frameLength();
        Serial.print(jpglen, DEC);
        Serial.println(" byte image");
        Serial.print("Writing image to "); 
        Serial.println(filename);
        Serial.print("Num of images: "); 
        Serial.println(timecount);
  
        while (jpglen > 0)
        {
            // read 32 bytes at a time;
            uint8_t *buffer;
            uint8_t bytesToRead = min(32, jpglen); // change 32 to 64 for a speedup but may not work with all setups!
            buffer = cam.readPicture(bytesToRead);
            imgFile.write(buffer, bytesToRead);
            delay(25);
            if(++Wcount>=64)
            {
                Serial.print("..."); // print something while transforming
                Wcount=0;
            }
            jpglen -= bytesToRead;
        }
        Serial.print("Delay");
        delay(10000);
        imgFile.close();
        Serial.println("...Done!");
        timecount+=1; // number of images
        writetxt=1; // txt can be write now
        if (timecount==20) //store a maximum of 20 times of data
        {
            timecount=0;
            SD.remove("Data.txt");
            for (int i = 0; i < 1000; i++) 
            {
                filename[5] = '0' + i/100;
                filename[6] = '0' + i%100/10;
                filename[7] = '0' + i%10;
                // create if does not exist, do not open existing, write, sync after write
                SD.remove(filename);
            }
        }
        cam.resumeVideo();
    }
    delay(4000);
    cam.begin();
}
