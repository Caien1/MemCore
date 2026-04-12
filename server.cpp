#include "./logger.cpp"
#include <arpa/inet.h>
#include <cstddef>
#include <cstdint>
#include <format>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct {
  size_t capacity;
  size_t size;
  uint8_t *buffer;
} Arena;

typedef struct {
  int port;
  int serverSocket;
  char buffer[4096];
  bool running; // TODO conccurrency thingy

} RServer;

void socketLog(RServer *server) {
  std::string runningStatus =
      server->running ? std::format("Runnin on port:{} ", server->port)
                      : "Not Running";

  logger(Severity::INFO, runningStatus);
}

void socketInit(RServer *server) {
  server->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (server->serverSocket < 0) {
    logger(Severity::ERROR, "Couldn't create Socket FD");
    exit(1);
  } else {
    logger(Severity::LOG, "Initialised Socket FD");
  }

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(server->port);
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  int bindSucess = bind(server->serverSocket, (struct sockaddr *)&serverAddr,
                        sizeof(struct sockaddr_in));
  if (bindSucess == 0) {
    logger(Severity::LOG, "Binded Address and port with socket");
  } else {
    logger(Severity::ERROR, "Could Bind Adress and port");
    exit(1);
  }

  int listenerSucess =
      listen(server->serverSocket, 5); // TODO ceck out this command in detail
                                       // and max simultaneous connections
  if (listenerSucess == 0) {
    logger(Severity::LOG, "Listening");
    server->running = true;
  } else {
    logger(Severity::ERROR, "Listner failed to Initialise");
    exit(1);
  }

  /*-------------unchecked------- */
  int optval = 1;
  setsockopt(server->serverSocket, SOL_SOCKET, SO_REUSEADDR, &optval,
             sizeof(int));
}

void socketAcceptLoop(RServer *server) {
  struct sockaddr_in clientAddr;
  socklen_t clientSize = sizeof(clientAddr);
  char buff2[] = "pong";
  for (;;) {
    int clientFd = accept(server->serverSocket, (struct sockaddr *)&clientAddr,
                          &clientSize);
    if (clientFd < 0) {
      logger(Severity::ERROR, "Error in accept");
      continue;
    }
    std::string msg =
        std::format("New connections from {}:{}",
                    inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    logger(Severity::INFO, msg);

    int recvLength = 0;

    while (true) {
      recvLength = recv(clientFd, server->buffer, sizeof(server->buffer), 0);
      if (recvLength < 0) {
        logger(Severity::ERROR, std::format("Recv Failed for {}:{}",
                                            inet_ntoa(clientAddr.sin_addr),
                                            ntohs(clientAddr.sin_port)));
        ;
        break;
      } else if (recvLength == 0) {
        logger(Severity::INFO, std::format("Client: {} : {} disconnected ",
                                           inet_ntoa(clientAddr.sin_addr),
                                           ntohs(clientAddr.sin_port)));
        break;
      } else {

        server->buffer[recvLength] = '\0';
        // TODO parse what is sent in the buffer
        logger(Severity::LOG, server->buffer);
        send(clientFd, buff2, sizeof(buff2), 0);
      }
    }
  }
}
void socketClose(RServer *server) { close(server->serverSocket); }
