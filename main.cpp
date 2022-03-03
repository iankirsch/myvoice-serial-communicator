#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "wiringPi/wiringPi.h"
#include "wiringPi/wiringSerial.h"

int main() {
  int serial_port;
  char data;
  if ((serial_port = serialOpen("/dev/ttyS0", 9600)) < 0) {
    fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
    return 1;
  }

  if (wiringPiSetup() == -1) {
    fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
    return 1;
  }

  while (1) {
    if (serialDataAvail(serial_port)) {
      data = serialGetchar(serial_port); /* receive character serially */
      printf("%c", data);
      fflush(stdout);
      // serialPutchar(serial_port, data); /* transmit character serially on port */
    }
  }
}