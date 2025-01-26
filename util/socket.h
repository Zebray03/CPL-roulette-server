#ifndef SOCKET_H
#define SOCKET_H

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

void send_message(SOCKET client, const char *message); // 发送消息

void receive_message(SOCKET client, char *buffer);    // 接收消息

#endif //SOCKET_H
