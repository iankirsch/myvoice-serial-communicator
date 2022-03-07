#pragma once
#include <string>

using namespace std;

class Logger {
 private:
  string ERROR_PREFIX;

 public:
  Logger();

  void log(string output);
  void error(string output);

  void welcome();

  void clear();
};