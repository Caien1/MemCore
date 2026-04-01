#include <iostream>
#include <string>
#include <unordered_map>
enum CommandOption { PING, SET, GET };
std::unordered_map<std::string, CommandOption> command_map = {
    {"ping", CommandOption::PING},
    {"set", CommandOption::SET},
    {"get", CommandOption::GET}};

std::unordered_map<std::string, std::string> db;

std::string command;
char delim = '\n';
int main() {
  for (;;) {
    // TODO empty string bug and and commad parsing and db ,stuff
    std::getline(std::cin, command, delim);
    CommandOption option = command_map[command];
    switch (option) {
    case CommandOption::PING:

      std::cout << "> PONG" << std::endl;
      break;
    case CommandOption::GET:

      std::cout << "> GET" << std::endl;
      break;

    case CommandOption::SET:

      std::cout << "> SET" << std::endl;
      break;
    default:

      std::cout << "> Not yet " << std::endl;
    }
  }
}
