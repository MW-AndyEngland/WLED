#include "wled.h"

#include <Wire.h>
#include <SPI.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <UWB.h>

/*
 * This v1 usermod file allows you to add own functionality to WLED more easily
 * See: https://github.com/Aircoookie/WLED/wiki/Add-own-functionality
 * EEPROM bytes 2750+ are reserved for your custom use case. (if you extend #define EEPSIZE in const.h)
 * If you just need 8 bytes, use 2551-2559 (you do not need to increase EEPSIZE)
 *
 * Consider the v2 usermod API if you need a more advanced feature set!
 */

//Use userVar0 and userVar1 (API calls &U0=,&U1=, uint16_t)
#define NUM_LEDS 12
#define LED_PIN 22

// for Display ************************
#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23

#define I2C_SDA 4
#define I2C_SCL 5


// WiFiClient client;
bool wifi_connected = false;

int send_rate = 100;


UWBDevice myUWB;
int index_num = 0;
unsigned long runtime = 0;

Adafruit_SSD1306 display(128, 64, &Wire, -1);


//gets called once at boot. Do all initialization that doesn't depend on network here
void userSetup()
{
    Serial.begin(115200);
    // display setup
    Wire.begin(I2C_SDA, I2C_SCL);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    display.clearDisplay();

    //logoShow(); //This can be uncommented once logoShow(); is placed and referenced in an external library

    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

    myUWB.initialize();
}

//gets called every time WiFi is (re-)connected. Initialize own network interfaces here
void userConnected()
{
    Serial.println("\nConnected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP()); 
    myUWB.udpInit(); // You might not need a specific local port for sending only
}

//loop. You can use "if (WLED_CONNECTED)" to check for successful connection
void userLoop()
{
    DW1000Ranging.loop();
    if ((millis() - runtime) > send_rate)
    {
        //display_uwb(uwb_data); //Still needs to be put into an external library
        if (WLED_CONNECTED)
        {
           myUWB.sendUWBData();
        }
        runtime = millis();
    }
}