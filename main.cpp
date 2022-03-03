#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wiringPi/wiringPi.h"
#include "wiringPi/wiringSerial.h"

int main() {
  extern void clear();

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

  // Clear console
  clear();

  // Print success message
  printf("Successfully connected to serial device on '/dev/ttyS0'.\nAwaiting data...\n");

  while (1) {
    if (serialDataAvail(serial_port)) {
      data = serialGetchar(serial_port); /* receive character serially */
      printf("%c", data);
      fflush(stdout);
      // serialPutchar(serial_port, data); /* transmit character serially on
      // port */
    }
  }
}

void clear() {
#if defined _WIN32
  system("cls");
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
  system("clear");
#elif defined(__APPLE__)
  system("clear");
#endif
}