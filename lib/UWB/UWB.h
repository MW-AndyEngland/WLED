#ifndef _DW1000Device_H_INCLUDED
#include <DW1000Device.h>
#endif
#include <DW1000Ranging.h>

#include <link.h>
//#include <OSCMessage.h>
#include <WiFiUdp.h>

#ifndef UWBDEVICE_h
#define UWBDEVICE_h
// for UWB ******************************
#define TAG_ADDR "7D:00:22:EA:82:60:3B:9B" // device 1 
// #define TAG_ADDR "8D:00:22:EA:82:60:3B:9B" //device 2
// #define TAG_ADDR "9D:00:22:EA:82:60:3B:9B" //device 3
#define UWB_NAME "MWUWB01"
// #define UWB_NAME "MWUWB02"

#define UWB_RST 27 // reset pin
#define UWB_IRQ 34 // irq pin
#define UWB_SS 21  // spi select pin

class UWBDevice {
    public:
        UWBDevice(); //Constructor here to instantiate objects
        void initialize(); // We may want to add a pointer to a MyLink object here so we can attach an external object to our private variable. I think we should also pass in Mac and name here but for now they're precompiler defines
        void udpInit();
        void newRange();
        void newDevice(DW1000Device *device);
        void inactiveDevice(DW1000Device *device);
        void sendOSCMessage(const String *msg_json);
        void sendUWBData();
    private:
        struct MyLink *uwb_data;
        String all_json = "";
        WiFiUDP Udp;
        const char *host = "192.168.0.30";
        const unsigned int outPort = 8080; // Update with your destination port
};

#endif