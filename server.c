#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
    int serverId = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverSocket;
    serverSocket.sin_port = htons(2020);
    serverSocket.sin_family = AF_INET;
    serverSocket.sin_addr.s_addr = INADDR_ANY;
    int opt = 1;

    setsockopt(serverId, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    int stat = bind(serverId, (const struct sockaddr*)&serverSocket, sizeof(serverSocket));
    
    listen(serverId, 5);

    char request[1024];
    char closed = 0;
    socklen_t len = sizeof(serverSocket);
    while (!closed) {
        int clientId = accept(serverId, (struct sockaddr*)&serverSocket, &len);
        int messageLen = read(clientId, request, 1023);
        if (messageLen > 0) {
            request[messageLen] = '\0'; // Null-terminate the string
            printf("New request:\n %s\n", request);
        }
        close(clientId);
    }
    close(serverId);
    return 0;
}
