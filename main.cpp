#include "./logger.cpp"
#include "./server.cpp"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <format>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int Port = 3000;

  if (argc >= 2) {
    // TODO validate port is a validate port
    Port = std::stoi(argv[1]);
    std::string msg =
        std::format("Port was supplied as an aggument {}", argv[1]);
    logger(Severity::INFO, msg);
  }

  RServer server = {};
  server.port = Port;

  socketInit(&server);
  acceptLoop(&server);
  socketClose(&server);

  return 0;
}
