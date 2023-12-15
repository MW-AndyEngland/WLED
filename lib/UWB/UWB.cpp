#include <UWB.h>

UWBDevice::UWBDevice()
{
    //Constructor!
}

void UWBDevice::initialize()
{
    // UWB init
    DW1000Ranging.initCommunication(UWB_RST, UWB_SS, UWB_IRQ); // Reset, CS, IRQ pin
    // Whatever these functions are doing, they aren't being called correctly. I don't think they should be returning void...
    //DW1000Ranging.attachNewRange(newRange());
    //DW1000Ranging.attachNewDevice(newDevice());
    //DW1000Ranging.attachInactiveDevice(inactiveDevice());
    
    // DW1000Ranging.startAsTag(TAG_ADDR, DW1000.MODE_SHORTDATA_FAST_ACCURACY); // MAC ADDY, RANGING MODE (SEE NOTES), randomize short_code bool
    // DW1000Ranging.startAsTag(TAG_ADDR, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);
    DW1000Ranging.startAsTag(TAG_ADDR, DW1000.MODE_LONGDATA_RANGE_ACCURACY);   
}

void UWBDevice::udpInit()
{
    Udp.begin(outPort);
}

void UWBDevice::newRange()
{
    char c[30];

    Serial.print("from: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
    Serial.print("\t Range: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getRange());
    Serial.print(" m");
    Serial.print("\t RX power: ");
    Serial.print(DW1000Ranging.getDistantDevice()->getRXPower());
    Serial.println(" dBm");
    fresh_link(uwb_data, DW1000Ranging.getDistantDevice()->getShortAddress(), DW1000Ranging.getDistantDevice()->getRange(), DW1000Ranging.getDistantDevice()->getRXPower());
}

void UWBDevice::newDevice(DW1000Device *device)
{
    Serial.print("ranging init; 1 device added ! -> ");
    Serial.print(" short:");
    Serial.println(device->getShortAddress(), HEX);

    add_link(uwb_data, device->getShortAddress());
}

void UWBDevice::inactiveDevice(DW1000Device *device)
{
    Serial.print("delete inactive device: ");
    Serial.println(device->getShortAddress(), HEX);

    delete_link(uwb_data, device->getShortAddress());
}

void UWBDevice::sendOSCMessage(const String *msg_json)
{
    /*Serial.println(" sending OSC ");
    String oscAddress = String(UWB_NAME) + "/anchor";
    OSCMessage msg(oscAddress.c_str()); // Convert the String object to a C-style string
                                        //    OSCMessage msg("/anchor");
    msg.add(msg_json->c_str());         // Convert String to C-style string
    Udp.beginPacket(host, outPort);     // Use host directly if it's a C-style string
    msg.send(Udp);                      // Send the OSC message
    Udp.endPacket();
    msg.empty(); // Clear the message for next use*/
}

void UWBDevice::sendUWBData()
{
    make_link_json(uwb_data, &all_json);
            //        send_udp(&all_json);
    sendOSCMessage(&all_json);
}