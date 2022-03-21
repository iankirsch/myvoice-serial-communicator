#pragma once
#include <string>

using namespace std;

class Logger {
 private:
  /**
   * @brief Prefix to use when displaying error messages
   */
  string ERROR_PREFIX;
  /**
   * @brief Prefix to use when displaying success messages
   */
  string SUCCESS_PREFIX;

 public:
  /**
   * @brief Construct a new Logger instance
   */
  Logger();

  /**
   * @brief Set to true to show debug messages
   */
  bool verbose;

  /**
   * @brief Log a string to stdout
   * @param input String to log
   */
  void log(string input);

  /**
   * @brief Log a string to stdout, only if verbose-mode is enabled
   * @param input String to log
   */
  void debug(string input);

  /**
   * @brief Log a string to stdout
   * Additionally, a success-indicating prefix is added.
   * @param input Success message to log
   */
  void success(string input);

  /**
   * @brief Log a string to stderror
   * Additionally, an error-indicating prefix is added.
   * @param input Error message to log
   */
  void error(string input);

  /**
   * @brief Add escape sequences to a string to make it appear bold in a terminal.
   * @return Bold text
   */
  string bold(string input);

  /**
   * @brief Add escape sequences to a string to make it appear black in a terminal.
   * @return Black text
   */
  string black(string input);

  /**
   * @brief Add escape sequences to a string to make it appear red in a terminal.
   * @return Red text
   */
  string red(string input);

  /**
   * @brief Add escape sequences to a string to make it appear green in a terminal.
   * @return Green text
   */
  string green(string input);

  /**
   * @brief Add escape sequences to a string to make it appear yellow in a terminal.
   * @return Yellow text
   */
  string yellow(string input);

  /**
   * @brief Add escape sequences to a string to make it appear blue in a terminal.
   * @return Blue text
   */
  string blue(string input);

  /**
   * @brief Add escape sequences to a string to make it appear magenta in a terminal.
   * @return Magenta text
   */
  string magenta(string input);

  /**
   * @brief Add escape sequences to a string to make it appear cyan in a terminal.
   * @return Cyan text
   */
  string cyan(string input);

  /**
   * @brief Add escape sequences to a string to make it appear white in a terminal.
   * @return White text
   */
  string white(string input);

  /**
   * @brief Prints a welcome message
   */
  void welcome();

  /**
   * @brief Clears the terminal screen
   */
  void clear();
};