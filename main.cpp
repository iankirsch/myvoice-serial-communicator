#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <chrono>
#include <iostream>

#include "lib/logger.hpp"
#include "wiringPi/wiringPi.h"
#include "wiringPi/wiringSerial.h"

using namespace std;

const char* HANDSHAKE_SEQUENCE = "--PING--\n";
const string HANDSHAKE_RESPONSE = "--PONG--";
int serial_port;

bool attemptHandshake();

Logger logger;

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

  // Clear console and show welcome screen
  logger.clear();
  logger.welcome();

  // Attempt handshake with Arduino
  if (!attemptHandshake()) {
    fprintf(stderr, "\nERROR: Connection to client device could not be established after 10s.\n");
    return 1;
  }

  // Print success message
  logger.log("\n\nSuccessfully connected to serial device on '/dev/ttyS0'.");
  logger.log("Awaiting data...");

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

  logger.log("Connecting... Please wait.");
  Time t0 = high_resolution_clock::now();

  bool handshakeReceived = false;
  bool sent = false;

  while (!handshakeReceived) {
    Time tNow = high_resolution_clock::now();
    int elapsedTime = duration_cast<milliseconds>(tNow - t0).count();

    if (serialDataAvail(serial_port)) {
      char character = serialGetchar(serial_port);
      int characterInt = character;

      if (characterInt == 13) {
        if (data == HANDSHAKE_RESPONSE) {
          handshakeReceived = true;
          logger.log("Handshake took " + to_string(elapsedTime) + " ms to complete.");
          break;
        } else {
          data = "";
        }
      } else {
        data += character;
      }
    }

    if (elapsedTime % 2000 == 0) {
      if (sent == false) {
        sent = true;
        serialPuts(serial_port, HANDSHAKE_SEQUENCE);
      }
    } else {
      sent = false;
    }

    if (elapsedTime > 10000) {
      return false;
    }
  }

  return true;
}