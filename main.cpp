#include "./logger.cpp"
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

  int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocketFd < 0) {
    logger(Severity::ERROR, "Couldn't create Socket FD");
    exit(1);
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
    exit(1);
  }

  int listenerSucess =
      listen(serverSocketFd, 5); // TODO ceck out this command in detail and max
                                 // simultaneous connections
  if (listenerSucess == 0) {
    logger(Severity::LOG,
           std::format("Listening at {} : {} ", inet_ntoa(serverAddr.sin_addr),
                       ntohs(serverAddr.sin_port)));
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
    std::string msg =
        std::format("New connections from {}:{}",
                    inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    logger(Severity::INFO, msg);

    int recvLength = 0;

    while (true) {
      recvLength = recv(clientFd, buffer, sizeof(buffer), 0);
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
        // TODO Fix logical bugs here
        //

        buffer[recvLength] = '\0';
        logger(Severity::LOG, buffer);
        send(clientFd, buff2, sizeof(buff2), 0);
      }
    }
  }
  close(serverSocketFd);
  return 0;
}
