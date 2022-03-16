#include "logger.hpp"

Logger::Logger() {
  this->ERROR_PREFIX = this->bold(this->red("ERROR: "));
  this->verbose = false;
  return;
}

void Logger::log(string input) {
  string finalinput = input + "\n";
  printf(finalinput.data());
}

void Logger::debug(string input) {
  if (this->verbose) {
    this->log(input);
  }
}

void Logger::error(string input) {
  string finalinput = this->ERROR_PREFIX + input + "\n";
  fprintf(stderr, finalinput.data());
}

string Logger::bold(string input) {
  return "\e[1m" + input + "\e[0m";
}

string Logger::black(string input) {
  return "\e[30m" + input + "\e[0m";
}

string Logger::red(string input) {
  return "\e[31m" + input + "\e[0m";
}

string Logger::green(string input) {
  return "\e[32m" + input + "\e[0m";
}

string Logger::yellow(string input) {
  return "\e[33m" + input + "\e[0m";
}

string Logger::blue(string input) {
  return "\e[34m" + input + "\e[0m";
}

string Logger::magenta(string input) {
  return "\e[35m" + input + "\e[0m";
}

string Logger::cyan(string input) {
  return "\e[36m" + input + "\e[0m";
}

string Logger::white(string input) {
  return "\e[37m" + input + "\e[0m";
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