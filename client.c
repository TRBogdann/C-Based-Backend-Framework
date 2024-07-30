#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void sendMessage(const char* message) {
    int serverId = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in socket;
    socket.sin_port = htons(2020);
    socket.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &socket.sin_addr);

    int stat1 = connect(serverId, (const struct sockaddr*)&socket, sizeof(socket));
    int stat2 = send(serverId, message, strlen(message), 0);

    close(serverId);
}

int main() {
    char buffer[1024];
    char closed = 0;
    while (!closed) {
        printf("Enter message: ");
        scanf("%1023s", buffer);
        if (strcmp(buffer, "exit") == 0)
            closed = 1;
        else
            sendMessage(buffer);
    }
    return 0;
}
