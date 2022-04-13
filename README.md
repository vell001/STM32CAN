
# STM32CAN

# This is an open source library I found a long time ago. I have made some modifications based on that open source library, but I can't find its real source, so I published it myself and published it on platformio:https://registry.platformio.org/libraries/vell001/STM32CAN

simple CAN library for stm32 devices using ST's official HAL layer, this include the ST supported Arduino

 it only support CAN1 and CAN2 as I don't have any devices with 3 CAN interfaces
 Before you can use this driver you need to enable the CAN module in the HAL configuration file,
	create a hal_conf_extra.h file in the root folder of the project and adding the following three lines:

#if !defined(HAL_CAN_MODULE_ENABLED)
#define HAL_CAN_MODULE_ENABLED
#endif

This library can be used in a C++ projects by replacing the
#include <Arduino.h> with the include for the used HAL for example;
#include <stm32f1xx_hal.h>
Inspired by the following giving me the input needed to make this work:
https://github.com/jiauka/stm32Can
https://gist.github.com/Arman92/154e2540847b32c44c29
https://github.com/collin80
ST's CAN examples and documentation
For CAN1 standard pins are ( PB_8 and PB_9) if set to false it use (PA_11 and PA_12)
For CAN2 standard pins are ( PB_5 and PB_6) if set to false it use (PB_13 and PB_13)
CAN message filtering:
This library use 32 bit IDMASK filtering
This is a good explanation of how mask and ID are handled in STM32 devices
https://schulz-m.github.io/2017/03/23/stm32-can-id-filter/
https://community.st.com/s/question/0D50X00009XkfSlSAJ/can-filters
More around standard and extended ID
http://www.copperhilltechnologies.com/can-bus-guide-extended-can-protocol/
Standard ID have a value between 0 and 0x7FF
Extended ID have a value between 0 and 0x1FFFFFFF

A CANBUS B frame (extended) consists of a four byte header (containing a 29-bit identifier), followed by up to 8 data bytes.
A receiving node would examine the identifier to decide if it was relevant (e.g. waiting for a frame with ID 00001567
which contains data to switch on or off a motor).
It could do this via software (using a C if or case statement); in practice the Canbus interface contains firmware to
carry out this task using the acceptance filter and mask value to filter out unwanted messages.
The filter mask is used to determine which bits in the identifier of the received frame are compared with the filter
If a mask bit is set to a zero, the corresponding ID bit will automatically be accepted, regardless of the value of the filter bit.
If a mask bit is set to a one, the corresponding ID bit will be compare with the value of the filter bit; if they
match it is accepted otherwise the frame is rejected.
Default this laibrary accept any frame e.g. no filters are applied
set filter to 0
set mask to 0
Example 1. we wish to accept only frames with ID of 00001567 (hexadecimal values)
set filter to 00001567
set mask to 1FFFFFFF
when a frame arrives its ID is compared with the filter and all bits must match; any frame that does not match ID 00001567 is rejected
Example 2. we wish to accept only frames with IDs of 00001560 thru to 0000156F
set filter to 00001560
set mask to 1FFFFFF0
when a frame arrives its ID is compared with the filter and all bits except bits 0 to 3 must match; any frame other frame is rejected
Example 3. we wish to accept only frames with IDs of 00001560 thru to 00001567
set filter to 00001560
set mask to 1FFFFFF8
when a frame arrives its ID is compared with the filter and all bits except bits 0 to 2 must match; any frame other frame is rejected
Library filter function:
bool setFilter( uint32_t FilterID, uint32_t FilterMask, uint8_t FilterBank, bool IDStdOrExt );
Please read the links to figure out FilterID and FilterMask
FilterBank have to be defined pr. CAN interface, 0 to 13 handle Can1 message filters and 14 to 27 handle Can1 message filters
You alway have to start with the default filter e.g. 0 for Can1 and 14 for Can2 as they by default is set to allow all messages
StdOrExt define ID type, default is standard
Example:
We would like to recive all CAN1 messages for std ID within range 0x400 thru to 0x40f
Can1.setFilter( 0x400, 0x7f0, 0, IDStd );
