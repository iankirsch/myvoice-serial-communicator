#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <chrono>
#include <fstream>
#include <iostream>

#include "lib/logger.hpp"
#include "wiringPi/wiringPi.h"
#include "wiringPi/wiringSerial.h"

using namespace std;

const char* HANDSHAKE_SEQUENCE = "--PING--\n";
const string HANDSHAKE_RESPONSE = "--PONG--";
const char* INTERRUPT_REQUEST = "--PLÖM--";

const string DATA_DIRECTORY = "output_data/";

int serial_port;

bool attemptHandshake();
string getFilepath(int fileNumber);
string getLineFromSerialPort(int serial_port);
void saveDataToFile(string filepath, string data);

Logger logger;

int main(int argc, char* argv[]) {
  string device = "/dev/ttyS0";
  // int rate = 115200;
  int rate = 1000000;
  bool noLogo = false;

  for (int i = 0; i < argc; ++i) {
    string arg = argv[i];
    if (arg == "--device") {
      if (argv[i + 1] == nullptr) {
        logger.error("No argument for flag " + logger.bold("--device") + " provided.");
        return 1;
      }
      device = argv[i + 1];
    }
    if (arg == "--rate") {
      if (argv[i + 1] == nullptr) {
        logger.error("No argument for flag " + logger.bold("--rate") + " provided.");
        return 1;
      }
      rate = atoi(argv[i + 1]);
    }
    if (arg == "--noLogo") {
      noLogo = true;
    }
  }

  // Clear console and show welcome screen
  if (!noLogo) {
    logger.clear();
    logger.welcome();
  }

  if ((serial_port = serialOpen(device.data(), rate)) < 0) {
    string error = strerror(errno);
    logger.error("Unable to open serial device: " + error);
    return 1;
  }

  if (wiringPiSetup() == -1) {
    string error = strerror(errno);
    logger.error("Unable to start wiringPi: " + error);
    return 1;
  }

  if (!noLogo) {
    logger.log(logger.bold("    Configured device: ") + logger.cyan(device));
    logger.log(logger.bold("    Configured rate: ") + logger.cyan(to_string(rate)));
    logger.log("");
  }

  // Attempt handshake with Arduino
  if (!attemptHandshake()) {
    fprintf(stderr, "\nERROR: Connection to client device could not be established after 10s.\n");
    return 1;
  }

  // Print success message
  logger.log("\n\nSuccessfully connected to serial device on '/dev/ttyS0'.");
  logger.log("Awaiting data...");

  // Keep track of curent line
  string currentLine = "";

  int fileNumber = 0;
  bool active = true;
  string collectedData = "";

  while (active) {
    string filename = getFilepath(fileNumber);

    if (serialDataAvail(serial_port)) {
      // Receive data serially
      string currentLine = getLineFromSerialPort(serial_port);
      logger.log(currentLine);

      if (currentLine == INTERRUPT_REQUEST) {
        logger.log("Saving sequence ...");

        string filepath = getFilepath(fileNumber++);
        saveDataToFile(filepath, collectedData);
        collectedData = "";

        logger.log("Saving complete!");
      } else {
        collectedData += currentLine + '\n';
      }
      fflush(stdout);
    }
  }
}

string getFilepath(int fileNumber) {
  string filename = DATA_DIRECTORY;

  filename += to_string(fileNumber);
  filename += ".txt";

  return filename;
}

void saveDataToFile(string filepath, string data) {
  ofstream file;
  file.open(filepath, std::ios_base::app);

  file << data;

  file.close();
}

string getLineFromSerialPort(int serial_port) {
  string line;
  char currentChar = serialGetchar(serial_port);

  while ((int)currentChar != 10) {
    line += currentChar;
    currentChar = serialGetchar(serial_port);
  }

  return line;
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

      if (characterInt == 10) {
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
