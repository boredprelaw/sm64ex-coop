#ifdef WINSOCK
#include <stdio.h>
#include "socket_windows.h"
#include "pc/debuglog.h"

SOCKET socket_initialize(void) {
    // start up winsock
    WSADATA wsaData;
    int rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (rc != NO_ERROR) {
        LOG_ERROR("WSAStartup failed with error %d", rc);
        return INVALID_SOCKET;
    }

    // initialize socket
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        LOG_ERROR("socket failed with error %d", SOCKET_LAST_ERROR);
        return INVALID_SOCKET;
    }

    // set non-blocking mode
    u_long iMode = 1;
    rc = ioctlsocket(sock, FIONBIO, &iMode);
    if (rc != NO_ERROR) {
        LOG_ERROR("ioctlsocket failed with error: %d", rc);
        return INVALID_SOCKET;
    }

    return sock;
}

void socket_shutdown(SOCKET socket) {
    if (socket == INVALID_SOCKET) { return; }
    int rc = closesocket(socket);
    if (rc == SOCKET_ERROR) {
        LOG_ERROR("closesocket failed with error %d", SOCKET_LAST_ERROR);
    }
    WSACleanup();
}

#endif
