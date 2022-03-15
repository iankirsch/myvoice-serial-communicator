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

  string bold(string output);
  string black(string output);
  string red(string output);
  string green(string output);
  string yellow(string output);
  string blue(string output);
  string magenta(string output);
  string cyan(string output);
  string white(string output);

  void welcome();

  void clear();
};