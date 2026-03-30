#include "./logger.cpp"
#include "logger.cpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
int main() {
  char adress[] = "localhost";
  int server = gethostname(adress, sizeof(adress));
  int clientFd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serverAddr;
  serverAddr.sin_port = htons(5000);
  serverAddr.sin_family = AF_INET;
  inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

  int cnn =
      connect(clientFd, (struct sockaddr *)&serverAddr, sizeof(sockaddr_in));

  char buff[1024] = "Hello there I'm ";
  int buffLen = strlen(buff);

  send(clientFd, buff, buffLen, 0);
  recv(clientFd, buff, sizeof(buff), 0);

  logger(Severity::LOG, buff);
  return 0;
}
