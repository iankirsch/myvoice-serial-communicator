#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "lib/common.hpp"
#include "lib/logger.hpp"
#include "wiringPi/wiringPi.h"
#include "wiringPi/wiringSerial.h"

using namespace std;

typedef chrono::high_resolution_clock::time_point Time;
using chrono::duration_cast;
using chrono::high_resolution_clock;
using chrono::milliseconds;

const char* HANDSHAKE_SEQUENCE = "--PING--\n";
const string HANDSHAKE_RESPONSE = "--PONG--";
const char* INTERRUPT_REQUEST = "--PLÖM--";

const string DATA_DIRECTORY = "output_data/";

string FILENAME_PREFIX = "unnamed-recording-";

int serial_port;

bool attemptHandshake();

string getFilepath(int fileNumber);
string getLineFromSerialPort(int serial_port);
void saveDataToFile(string filepath, string data);

Logger logger;

int main(int argc, char* argv[]) {
  string device = "/dev/ttyS0";
  int rate = 1000000;  // 115200;
  int granularity = 100;
  int recordingTime = 2000;
  bool noLogo = false;

  // Apply properties from flags
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
    if (arg == "--granularity") {
      if (argv[i + 1] == nullptr) {
        logger.error("No argument for flag " + logger.bold("--granularity") + " provided.");
        return 1;
      }
      granularity = atoi(argv[i + 1]);
    }
    if (arg == "--recordingTime") {
      if (argv[i + 1] == nullptr) {
        logger.error("No argument for flag " + logger.bold("--recordingTime") + " provided.");
        return 1;
      }
      recordingTime = atoi(argv[i + 1]);
    }
    if (arg == "--name") {
      if (argv[i + 1] == nullptr) {
        logger.error("No argument for flag " + logger.bold("--name") + " provided.");
        return 1;
      }
      FILENAME_PREFIX = argv[i + 1];
    }
    if (arg == "--noLogo") {
      noLogo = true;
    }
    if (arg == "--verbose" || arg == "-v") {
      logger.verbose = true;
    }
  }

  // Clear console and show welcome screen
  if (!noLogo) {
    logger.clear();
    logger.welcome();
  }

  // Connect to serial device
  if ((serial_port = serialOpen(device.data(), rate)) < 0) {
    string error = strerror(errno);
    logger.error("Unable to open serial device: " + error);
    return 1;
  }

  // Start wiringPi library
  if (wiringPiSetup() == -1) {
    string error = strerror(errno);
    logger.error("Unable to start wiringPi: " + error);
    return 1;
  }

  // Show configuration
  if (!noLogo) {
    logger.log(logger.bold("    Configured device: ") + logger.cyan(device));
    logger.log(logger.bold("    Configured rate: ") + logger.cyan(to_string(rate)));
    logger.log(logger.bold("    Configured granularity: ") + logger.cyan(to_string(granularity) + "ms"));
    logger.log(logger.bold("    Configured recording time: ") + logger.cyan(to_string(recordingTime) + "ms"));
    logger.log(logger.bold("    Configured recording name: ") + logger.cyan(FILENAME_PREFIX));
    logger.debug(logger.yellow("\n    Verbose mode enabled. "));
    logger.log("");
  }

  // Attempt handshake with Arduino
  if (!attemptHandshake()) {
    logger.error("Connection to client device could not be established after 10s.");
    return 1;
  }

  // Print success message
  logger.log("");
  logger.success("Connected to serial device on '" + device + "'.");
  logger.debug("Awaiting data...");

  // Keep track of current state
  bool active = true;
  int fileNumber = 0;

  string currentLine = "";

  int receivedValuesCount = 0;
  vector<int> tempReceivedValues = {0, 0, 0, 0, 0, 0, 0};
  vector<vector<int>> collectedValues;

  Time t0 = high_resolution_clock::now();

  while (active) {
    string filename = getFilepath(fileNumber);

    if (serialDataAvail(serial_port)) {
      // Receive data serially
      string currentLine = getLineFromSerialPort(serial_port);

      logger.debug(currentLine);

      if (currentLine[0] == '[' && currentLine[currentLine.length() - 1] == ']') {
        // Received line starts with "[" and ends with "]", indicating that this is important data
        Time tNow = high_resolution_clock::now();
        int elapsedTime = duration_cast<milliseconds>(tNow - t0).count();

        // Split data to vector
        vector<string> receivedStringValues = split(currentLine.substr(1, currentLine.length() - 2), ",");
        vector<int> receivedValues = stringVectorToIntVector(receivedStringValues);

        for (int i = 0; i < receivedValues.size(); i++) {
          tempReceivedValues[i] += receivedValues[i];
        }

        receivedValuesCount++;

        // Average received data every "granularity" ms
        if (elapsedTime >= granularity) {
          logger.debug("Averaging");
          for (int i = 0; i < tempReceivedValues.size(); i++) {
            tempReceivedValues[i] = tempReceivedValues[i] / receivedValuesCount;
          }

          collectedValues.push_back(tempReceivedValues);

          t0 = high_resolution_clock::now();
          tempReceivedValues = {0, 0, 0, 0, 0, 0, 0};
          receivedValuesCount = 0;
        }
      }

      if (currentLine == INTERRUPT_REQUEST) {
        // Received line is interrupt request, save sequence and clear recorded numbers.
        logger.debug("Saving sequence ...");
        int saveLength = recordingTime / granularity;

        if (collectedValues.size() < saveLength) {
          logger.error("Not enough values captured for configured recording time and granularity. Please wait and try again.");
          collectedValues.clear();
        } else {
          vector<vector<int>>::const_iterator first = collectedValues.end() - saveLength;
          vector<vector<int>>::const_iterator last = collectedValues.end();

          vector<vector<int>> recordedValues(first, last);

          string filepath = getFilepath(fileNumber++);
          saveDataToFile(filepath, formatCollectedValues(recordedValues));
          collectedValues.clear();

          logger.success("Saving complete!");
        }
      }
      fflush(stdout);
    }
  }
}

string getFilepath(int fileNumber) {
  string filename = DATA_DIRECTORY;

  filename += FILENAME_PREFIX;
  if (FILENAME_PREFIX.back() != '-') {
    filename += "-";
  }
  filename += to_string(fileNumber);
  filename += ".txt";

  return filename;
}

void saveDataToFile(string filepath, string data) {
  ofstream file;
  file.open(filepath);

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
          logger.debug("Handshake took " + to_string(elapsedTime) + " ms to complete.");
          break;
        } else {
          data = "";
        }
      } else {
        data += character;
      }
    }

    if (elapsedTime % 500 == 0) {
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
