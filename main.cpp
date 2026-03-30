#include "./logger.cpp"
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <cmath>
#include <ctime>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

typedef struct RedisServer {
  int port;
  int serverSocket;
  bool running; // TODO change to atomic bool later for concurrency issues

  RedisServer(int port) { this->port = port; }

  void log() {
    std::cout << "Redis Server log" << std::endl;
    std::cout << "Port " << this->port << std::endl;
  }

  void socketInit() {

    int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocketFd < 0) {
      logger(Severity::ERROR, "Couldn't create Socket FD");
    } else {
      logger(Severity::LOG, "Initialised Socket FD");
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(this->port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    int bindSucess = bind(serverSocketFd, (struct sockaddr *)&serverAddr,
                          sizeof(struct sockaddr_in));

    if (bindSucess == 0) {
      logger(Severity::LOG, "Binded Address and port with socket");
    } else {
      logger(Severity::ERROR, "Could Bind Adress and port");
    }

    int listenerSucess =
        listen(serverSocketFd, 5); // TODO ceck out this command in detail and
                                   // max simultaneous connections
    if (listenerSucess == 0) {
      logger(Severity::LOG, " Listening");
    } else {
      logger(Severity::ERROR, "Listner failed");
    }
  }

} RServer;

int main(int argc, char *argv[]) {
  int Port = 3000;

  if (argc >= 2) {
    // TODO validate port is a validate port
    Port = std::stoi(argv[1]);
    std::string msg = "Port set to " + std::string(argv[1]);
    std::cout << msg << std::endl;
    logger(Severity::LOG, msg);
  }

  RServer instance = RedisServer(Port);
  instance.log();

  int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocketFd < 0) {
    logger(Severity::ERROR, "Couldn't create Socket FD");
  } else {
    logger(Severity::LOG, "Initialised Socket FD");
  }

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(Port);
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  int bindSucess = bind(serverSocketFd, (struct sockaddr *)&serverAddr,
                        sizeof(struct sockaddr_in));
  if (bindSucess == 0) {
    logger(Severity::LOG, "Binded Address and port with socket");
  } else {
    logger(Severity::ERROR, "Could Bind Adress and port");
  }

  int listenerSucess =
      listen(serverSocketFd, 5); // TODO ceck out this command in detail and max
                                 // simultaneous connections
  if (listenerSucess == 0) {
    logger(Severity::LOG, "Listening");
  } else {
    logger(Severity::ERROR, "Listner failed to Initialise");
  }
  // TODO check serveropt() function
  struct sockaddr_in clientAddr;
  socklen_t clientSize = sizeof(clientAddr);

  /*-------------unchecked------- */
  int optval = 1;
  setsockopt(serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

  /*-------------unchecked------- */

  char buffer[1024];
  char buff2[] = "pong";
  for (;;) {
    int clientFd =
        accept(serverSocketFd, (struct sockaddr *)&clientAddr, &clientSize);
    if (clientFd < 0) {
      logger(Severity::ERROR, "Error in accept");
      continue;
    }
    std::string msg = "New connections from " +
                      (std::string)inet_ntoa(clientAddr.sin_addr) + " Port" +
                      std::to_string(ntohs(clientAddr.sin_port));
    logger(Severity::LOG, msg);

    int recvLength = recv(clientFd, buffer, sizeof(buffer), 0);
    if (recvLength < 0) {
      logger(Severity::ERROR, "recv() Failed");
      close(clientFd);
    } else {
      buffer[recvLength] = '\0';
      logger(Severity::LOG, buffer);
    }

    send(clientFd, buff2, sizeof(buff2), 0);
  }

  close(serverSocketFd);
  return 0;
}
