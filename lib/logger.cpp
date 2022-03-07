#include "logger.hpp"

Logger::Logger() {
  this->ERROR_PREFIX = "ERROR: ";
  return;
}

void Logger::log(string output) {
  string finalOutput = output + "\n";
  printf(finalOutput.data());
}

void Logger::error(string output) {
  string finalOutput = this->ERROR_PREFIX + output + "\n";
  fprintf(stderr, finalOutput.data());
}

void Logger::welcome() {
  printf("  _____           _       _    _____                                      _           _\n");
  printf(" / ____|         (_)     | |  / ____|                                    (_)         | |\n");
  printf("| (___   ___ _ __ _  __ _| | | |     ___  _ __ ___  _ __ ___  _   _ _ __  _  ___ __ _| |_ ___  _ __\n");
  printf(" \\___ \\ / _ | '__| |/ _` | | | |    / _ \\| '_ ` _ \\| '_ ` _ \\| | | | '_ \\| |/ __/ _` | __/ _ \\| '__|\n");
  printf(" ____) |  __| |  | | (_| | | | |___| (_) | | | | | | | | | | | |_| | | | | | (_| (_| | || (_) | |\n");
  printf("|_____/ \\___|_|  |_|\\__,_|_|  \\_____\\___/|_| |_| |_|_| |_| |_|\\__,_|_| |_|_|\\___\\__,_|\\__\\___/|_|\n\n");
}

void Logger::clear() {
#if defined _WIN32
  system("cls");
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
  system("clear");
#elif defined(__APPLE__)
  system("clear");
#endif
}