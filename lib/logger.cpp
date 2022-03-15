#include "logger.hpp"

Logger::Logger() {
  this->ERROR_PREFIX = this->bold(this->red("ERROR: "));
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

string Logger::bold(string output) {
  return "\e[1m" + output + "\e[0m";
}

string Logger::black(string output) {
  return "\e[30m" + output + "\e[0m";
}

string Logger::red(string output) {
  return "\e[31m" + output + "\e[0m";
}

string Logger::green(string output) {
  return "\e[32m" + output + "\e[0m";
}

string Logger::yellow(string output) {
  return "\e[33m" + output + "\e[0m";
}

string Logger::blue(string output) {
  return "\e[34m" + output + "\e[0m";
}

string Logger::magenta(string output) {
  return "\e[35m" + output + "\e[0m";
}

string Logger::cyan(string output) {
  return "\e[36m" + output + "\e[0m";
}

string Logger::white(string output) {
  return "\e[37m" + output + "\e[0m";
}


void Logger::welcome() {
  this->log(this->bold("  _____           _       _    _____                                      _           _"));
  this->log(this->bold(" / ____|         (_)     | |  / ____|                                    (_)         | |"));
  this->log(this->bold("| (___   ___ _ __ _  __ _| | | |     ___  _ __ ___  _ __ ___  _   _ _ __  _  ___ __ _| |_ ___  _ __"));
  this->log(this->bold(" \\___ \\ / _ | '__| |/ _` | | | |    / _ \\| '_ ` _ \\| '_ ` _ \\| | | | '_ \\| |/ __/ _` | __/ _ \\| '__|"));
  this->log(this->bold(" ____) |  __| |  | | (_| | | | |___| (_) | | | | | | | | | | | |_| | | | | | (_| (_| | || (_) | |"));
  this->log(this->bold("|_____/ \\___|_|  |_|\\__,_|_|  \\_____\\___/|_| |_| |_|_| |_| |_|\\__,_|_| |_|_|\\___\\__,_|\\__\\___/|_|\n"));
}

void Logger::clear() {
#if defined _WIN32
  system("cls");
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__) || defined(__APPLE__)
  system("clear");
#endif
}