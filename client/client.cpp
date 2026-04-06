#include "../logger.cpp"
#include "raylib_namespace.h"
#include <arpa/inet.h>
#include <cstdlib>
#include <netinet/in.h>
#include <raylib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
typedef struct {
  int port;
  char address[16];
  char *message;
} Client;
void sendRequest() {}

int main(int argc, char *argv[]) {

  char ip[16] = {0};
  char port[6] = {0};

  char buff[1024] = "Hello there I'm ";
  char panelBuff[1024] = {0};
  int buffLen = strlen(buff);

  // Create socket once, outside the loop
  int clientFd = socket(AF_INET, SOCK_STREAM, 0);
  if (clientFd == -1) {
    // Handle error
    return -1;
  }

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  bool connected = false;

  InitWindow(800, 400, "Client");
  SetTargetFPS(60);

  Rectangle ipTextBox = {200, 20, 150, 30};
  Rectangle portTextBox = {ipTextBox.x + ipTextBox.width + 5, 20, 50, 30};
  Rectangle goButton = {portTextBox.x + portTextBox.width + 5, 20, 30, 30};
  Rectangle panelRect = {10, 200, 780, 250};
  Rectangle resultRect = {10, 230, 780, 180};
  Rectangle messageRect = {ipTextBox.x, ipTextBox.y + 40, 230, 30};

  bool ipEditMode = false;
  bool portEditMode = false;
  bool messageEditMode = false;

  // Input state
  bool portEntered = false;

  while (!WindowShouldClose()) {
    BeginDrawing();

    if (CheckCollisionPointRec(GetMousePosition(), ipTextBox) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      ipEditMode = true;
    } else if (CheckCollisionPointRec(GetMousePosition(), portTextBox) &&
               IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      portEditMode = true;

    } else if (CheckCollisionPointRec(GetMousePosition(), messageRect) &&
               IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      messageEditMode = true;
    }

    if (GuiTextBox(ipTextBox, ip, 15, ipEditMode)) {
      ipEditMode = false;
    }

    if (GuiTextBox(portTextBox, port, 5, portEditMode)) {
      portEditMode = false;
    }

    if (GuiTextBox(messageRect, buff, sizeof(buff), messageEditMode)) {
      messageEditMode = false;
    }

    if (GuiButton(goButton, "Go")) {
      if (!connected) {
        inet_pton(AF_INET, ip, &serverAddr.sin_addr);
        serverAddr.sin_port = htons(std::stoi(port));

        if (connect(clientFd, (struct sockaddr *)&serverAddr,
                    sizeof(sockaddr_in)) == 0) {
          connected = true;
          send(clientFd, buff, buffLen, 0);
          recv(clientFd, panelBuff, sizeof(panelBuff), 0);
          logger(Severity::LOG, buff);
        } else {
          logger(Severity::ERROR, "Connection failed");
        }
      } else {
        send(clientFd, buff, buffLen, 0);
        recv(clientFd, panelBuff, sizeof(panelBuff), 0);
        logger(Severity::LOG, buff);
      }
    }

    GuiPanel(panelRect, "Result");

    GuiTextBox(resultRect, panelBuff, sizeof(panelBuff), false);

    ClearBackground(RAYWHITE);

    EndDrawing();
  }
  return 0;
}
