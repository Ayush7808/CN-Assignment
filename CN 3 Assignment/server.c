#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    // Listen for connections
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept connection
    new_socket = accept(server_fd,
                        (struct sockaddr *)&address,
                        (socklen_t *)&addrlen);

    if (new_socket < 0) {
        perror("Accept failed");
        exit(1);
    }

    printf("Client connected.\n");

    // Communication loop
    while (1) {

        // Clear buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Receive message from client
        int bytes_received = recv(new_socket, buffer,
                                  BUFFER_SIZE - 1, 0);

        if (bytes_received <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        buffer[bytes_received] = '\0';

        printf("Client: %s\n", buffer);

        // Check for exit command
        if (strcmp(buffer, "exit") == 0) {
            printf("Client ended the connection.\n");
            break;
        }

        // Get message from server user
        printf("Server: ");
        fgets(message, BUFFER_SIZE, stdin);

        // Remove newline
        message[strcspn(message, "\n")] = '\0';

        // Send message to client
        send(new_socket, message, strlen(message), 0);

        // Check for exit command
        if (strcmp(message, "exit") == 0) {
            printf("Server ended the connection.\n");
            break;
        }
    }

    // Close sockets
    close(new_socket);
    close(server_fd);

    return 0;
}