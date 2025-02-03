#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 3000
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE] = {0};

    // Step 1: Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Step 2: Bind the socket to the address and port
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return -1;
    }

    // Step 3: Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        return -1;
    }

    std::cout << "Server is listening on port " << PORT << "...\n";

    // Step 4: Accept a connection from the client
    if ((new_socket = accept(server_fd, NULL, NULL)) < 0) {
        perror("Accept failed");
        return -1;
    }

    std::cout << "Connection established\n";

    std::string message;
    while (true) {
        // Step 5: Receive data from client
        memset(buffer, 0, sizeof(buffer));  // Clear the buffer
        read(new_socket, buffer, BUFFER_SIZE);
        std::cout << "Client: " << buffer << "\n";

        // Check if client sent "bye" to terminate
        if (strcmp(buffer, "bye") == 0) {
            std::cout << "Client disconnected\n";
            break;
        }

        // Step 6: Send response to client
        std::cout << "Enter message for client: ";
        std::getline(std::cin, message);
        send(new_socket, message.c_str(), message.length(), 0);

        // Check if the server wants to exit the loop
        if (message == "bye") {
            std::cout << "Server disconnected\n";
            break;
        }
    }

    // Step 7: Close the socket
    close(new_socket);
    close(server_fd);

    return 0;
}
