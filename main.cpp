#include "MicroBit.h"
#include "MicroBitUARTService.h"

MicroBit uBit;
MicroBitUARTService *uart;


int connected = 0;

void onConnected(MicroBitEvent)
{

    uBit.display.scroll("C");

    connected = 1;

    // mobile app will send ASCII strings terminated with the colon character
    ManagedString eom(":");

    while(connected == 1) {
        ManagedString msg = uart->readUntil(eom);
        uBit.display.scroll(msg);
    }

}

void onDisconnected(MicroBitEvent)
{
    uBit.display.scroll("D");
    connected = 0;
}

void onButtonA(MicroBitEvent)
{
    if (connected == 0) {
        return;
    }
    uart->send(ManagedString("Connected"));
    uBit.display.scroll("Y");
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

	uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, onConnected);
	uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, onDisconnected);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);


    uart = new MicroBitUARTService(*uBit.ble, 32, 32);
    uBit.display.scroll("Ready");

	release_fiber();
}