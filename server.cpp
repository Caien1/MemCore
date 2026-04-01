#include "./logger.cpp"
#include <arpa/inet.h>
#include <format>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct RedisServer {
  int port;
  int serverSocket;
  bool running; // TODO change to atomic bool later for concurrency issues

  RedisServer(int port) : port(port), serverSocket(0), running(false) {}

  void log() {
    std::string runningStatus =
        this->running ? std::format("Runnin on port:{} ", this->port)
                      : "Not Running";

    logger(Severity::INFO, runningStatus);
  }

  void socketInit() {
    int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocketFd < 0) {
      logger(Severity::ERROR, "Couldn't create Socket FD");
      exit(1);
    } else {
      logger(Severity::LOG, "Initialised Socket FD");
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(this->port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int bindSucess = bind(serverSocketFd, (struct sockaddr *)&serverAddr,
                          sizeof(struct sockaddr_in));
    if (bindSucess == 0) {
      logger(Severity::LOG, "Binded Address and port with socket");
    } else {
      logger(Severity::ERROR, "Could Bind Adress and port");
      exit(1);
    }

    int listenerSucess =
        listen(serverSocketFd, 5); // TODO ceck out this command in detail and
                                   // max simultaneous connections
    if (listenerSucess == 0) {
      logger(Severity::LOG, "Listening");
    } else {
      logger(Severity::ERROR, "Listner failed to Initialise");
      exit(1);
    }
    // TODO check serveropt() function
    struct sockaddr_in clientAddr;
    socklen_t clientSize = sizeof(clientAddr);

    /*-------------unchecked------- */
    int optval = 1;
    setsockopt(serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
  }

  void socketClose() { close(this->serverSocket); }

} RServer;
