#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unordered_map>

#include <unistd.h>
enum Severity { LOG, WARNING, ERROR };
std::unordered_map<Severity, std::string> SeverityMap = {
    {Severity::LOG, "[LOG]"},
    {Severity::WARNING, "[WARNING]"},
    {Severity::ERROR, "[ERROR]"},
};

void loger(Severity level, std::string msg) {
  // TODO Add proper time-stamps and pipe logs to a file
  std::string tab[3] = {"[+]", "[*]", "[=]"};
  std::cout << tab[level] << SeverityMap[level] << " " << msg << std::endl;
}

typedef struct RedisServer {
  int port;
  int serverSocket;
  bool running; // TODO change to atomic bool later for concurrency issues

  RedisServer(int port) { this->port = port; }
  void log() {
    std::cout << "Redis Server log" << std::endl;
    std::cout << "Port " << this->port << std::endl;
  }

} RServer;

int main(int argc, char *argv[]) {
  int Port = 3000;

  if (argc >= 2) {
    // TODO validate port is a validate port
    Port = std::stoi(argv[1]);
    std::string msg = "Port set to " + std::string(argv[1]);
    std::cout << msg << std::endl;
    loger(Severity::LOG, msg);
  }

  RServer instance = RedisServer(Port);
  instance.log();

  int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocketFd < 0) {
    loger(Severity::ERROR, "Couldn't create Socket FD");
  } else {
    loger(Severity::LOG, "Initialised Socket FD");
  }

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(Port);
  serverAddr.sin_addr.s_addr = INADDR_ANY;

  int bindSucess = bind(serverSocketFd, (struct sockaddr *)&serverAddr,
                        sizeof(struct sockaddr_in));
  if (bindSucess == 0) {
    loger(Severity::LOG, "Binded Address and port with socket");
  } else {
    loger(Severity::ERROR, "Could Bind Adress and port");
  }

  int listenerSucess =
      listen(serverSocketFd, 5); // TODO ceck out this command in detail and max
                                 // simultaneous connections
  if (listenerSucess == 0) {
    loger(Severity::LOG, " Listening");
  } else {
    loger(Severity::ERROR, "Listner failed");
  }

  struct sockaddr_in clientAddr;
  socklen_t clientSize = sizeof(clientAddr);
  int acceptFd =
      accept(serverSocketFd, (struct sockaddr *)&clientAddr, &clientSize);
  close(serverSocketFd);

  return 0;
}
