#include <STM32CAN.h>
void setup()
{
    Serial1.begin(115200);
    Serial1.println("Hi it's 1\n\r");
    int ret = Can1.begin(); //Use std pins (PB_8 and PB_9)
    while (ret!=0) {
        Serial1.print("can error: ");
        Serial1.println(ret);
        ret = Can1.begin();
    }
//    Can1.begin(CAN_DEFAULT_BAUD, false, false); //Use alt pins ( PA_11 and PA_12)

}

void printFrame(CAN_message_t &frame) {
    Serial1.print("ID: 0x");
    Serial1.print(frame.id, HEX);
    Serial1.print(" Len: ");
    Serial1.print(frame.dlc);
    Serial1.print(" Data: 0x");
    for (int count = 0; count < frame.dlc; count++) {
        Serial1.print(frame.data.bytes[count], HEX);
        Serial1.print(" ");
    }
    Serial1.print("\r\n");
}

void loop(){
    delay(1000);
    CAN_message_t incoming;
    incoming.id = 0x11;
    incoming.data.value = 1000;
    bool ret = Can1.write(incoming);
    Serial1.print("send ret: ");
    Serial1.println(ret);

    if (Can1.available() > 0) {
        Serial1.println("available");
        Can1.read(incoming);
        printFrame(incoming);
    }
}