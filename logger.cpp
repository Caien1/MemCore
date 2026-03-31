#pragma once
#include <iostream>

#include <cmath>
#include <ctime>

enum Severity { LOG, WARNING, ERROR };
void logger(Severity level, std::string msg) {

  // TODO check if this method of making time stamps is accurate and append logs
  // to a file
  time_t now = time(nullptr);
  struct tm *tm_info = localtime(&now);
  char buffer[20];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);

  std::string SeverityMap[3] = {"[LOG]", "[WARNING]", "ERROR"};
  std::string tab[3] = {"[+]", "[*]", "[=]"};

  std::cout << tab[level] << SeverityMap[level] << " Timestamp: " << buffer
            << " " << msg << std::endl;
}

// TODO implement better logging system
//
