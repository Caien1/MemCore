#pragma once
#include <iostream>

#include <cmath>
#include <ctime>

enum Severity { LOG, WARNING, ERROR, INFO };
void logger(Severity level, std::string msg) {

  // TODO check if this method of making time stamps is accurate and append logs
  // to a file
  time_t now = time(nullptr);
  struct tm *tm_info = localtime(&now);
  char buffer[20];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
  // NOTE Use isatty(STDOUT_FILENO) to detect terminal support and disable
  // colors when piping output.
  std::string SeverityMap[4] = {
      "\033[33;1m[LOG]\033[0m", "\033[035m[WARNING]\033[0m",
      "\033[31;1m[ERROR]\033[0m", "\033[34;4m[INFO]\033[0m"};
  std::string tab[4] = {"[+]", "[*]", "[=]", "[!]"};

  std::cout << tab[level] << SeverityMap[level] << " Timestamp: " << buffer
            << " " << msg << std::endl;
}

// TODO implement better logging system
//
