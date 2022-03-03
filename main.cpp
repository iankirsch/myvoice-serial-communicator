#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <chrono>
#include <iostream>
#include <string>

#include "wiringPi/wiringPi.h"
#include "wiringPi/wiringSerial.h"

using namespace std;

const char* HANDSHAKE_SEQUENCE = "--PING--\n";
const string HANDSHAKE_RESPONSE = "--PONG--";
int serial_port;

void clear();
bool attemptHandshake();

int main() {
  char data;

  if ((serial_port = serialOpen("/dev/ttyS0", 9600)) < 0) {
    fprintf(stderr, "\nERROR: Unable to open serial device: %s\n", strerror(errno));
    return 1;
  }

  if (wiringPiSetup() == -1) {
    fprintf(stdout, "\nERROR: Unable to start wiringPi: %s\n", strerror(errno));
    return 1;
  }

  // Clear console
  clear();

  // Attempt handshake with Arduino
  if (!attemptHandshake()) {
    fprintf(stderr, "\nERROR: Connection to client device could not be established after 10s.\n");
    return 1;
  }

  // Print success message
  printf("\n\nSuccessfully connected to serial device on '/dev/ttyS0'.\n");
  printf("Awaiting data...\n");

  while (true) {
    if (serialDataAvail(serial_port)) {
      data = serialGetchar(serial_port); /* receive character serially */
      printf("%c", data);
      fflush(stdout);
      // serialPutchar(serial_port, data); /* transmit character serially on
      // port */
    }
  }
}

bool attemptHandshake() {
  typedef chrono::high_resolution_clock::time_point Time;
  using chrono::duration_cast;
  using chrono::high_resolution_clock;
  using chrono::milliseconds;

  string data;

  printf("Connecting... Please wait.\n");
  Time t0 = high_resolution_clock::now();
  serialPuts(serial_port, HANDSHAKE_SEQUENCE);

  bool handshakeReceived = false;
  while (!handshakeReceived) {
    Time tNow = high_resolution_clock::now();
    int elapsedTime = duration_cast<milliseconds>(tNow - t0).count();

    if (serialDataAvail(serial_port)) {
      char character = serialGetchar(serial_port);
      int characterInt = character;

      if (characterInt == 13) {
        if (data == HANDSHAKE_RESPONSE) {
          handshakeReceived = true;
          printf("Handshake took %s ms to complete.\n", to_string(elapsedTime).data());
          break;
        } else {
          data = "";
        }
      } else {
        data += character;
      }
    }
    if (elapsedTime > 10000) {
      return false;
    }
  }

  return true;
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