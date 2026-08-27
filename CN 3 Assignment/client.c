#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr,
                sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to server.\n");

    // Communication loop
    while (1) {

        // Get message from client/user
        printf("Client: ");
        fgets(message, BUFFER_SIZE, stdin);

        // Remove newline
        message[strcspn(message, "\n")] = '\0';

        // Send message to server
        send(sock, message, strlen(message), 0);

        // Check for exit
        if (strcmp(message, "exit") == 0) {
            printf("Connection closed.\n");
            break;
        }

        // Clear buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Receive message from server
        int bytes_received = recv(sock, buffer,
                                  BUFFER_SIZE - 1, 0);

        if (bytes_received <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        buffer[bytes_received] = '\0';

        printf("Server: %s\n", buffer);
    }

    close(sock);

    return 0;
}
