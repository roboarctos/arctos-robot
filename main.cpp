/*
 * main.c
 *
 * Created on: 22.10.15
 *     Author: Nino Schoch
 */
#include <sys/fcntl.h>
#include <string.h>
#include "communication/serial.h"
#include "communication/Bluetooth.h"
#include "Controller.h"
#include "communication/rfid.h"

int main(int argc, char* argv[]) {

    // Get the input params
    char* bluetooth_port = (char*) "/dev/pts/6";
    char* rfid_port = (char*) "/dev/ttyUSB0";
    if (argc > 1) {
        bluetooth_port = argv[1];
    }
    if (argc > 2) {
        rfid_port = argv[2];
    }

    //robot_init();
    //robot_drive_left(250);
    //robot_turn_left();

    robot_options_t options;
    options.callbacks.controller_rfid_init = rfid_init;
    options.callbacks.controller_rfid_read = rfid_read;
    options.serial_port_options_rfid.port_name = rfid_port;
    options.serial_port_options_rfid.speed = B2400;

    serial_port_options_t bluetooth_options;
    bluetooth_options.port_name = bluetooth_port;
    bluetooth_options.speed = B115200;

    ProtocolLayer* protocol = new ApplicationLayer(
            new PresentationLayer(
                    new SessionLayer(
                            new TransportLayer(
                                    new Bluetooth(&bluetooth_options)
                            )
                    )
            )
    );
    Controller* controller = new Controller(&options, protocol);
    controller->start();
    delete(controller);
    delete(protocol);

    //robot_disconnect();
    return 0;
}