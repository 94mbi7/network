#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 3000
#define SERVER_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE] = {0};

    // Step 1: Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, SERVER_ADDRESS, &server_address.sin_addr) <= 0) {
        perror("Invalid address");
        return -1;
    }

    // Step 2: Connect to the server
    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        return -1;
    }

    std::cout << "Connected to the server\n";

    std::string message;
    while (true) {
        // Step 3: Send data to the server
        std::cout << "Enter message for server: ";
        std::getline(std::cin, message);
        send(sock, message.c_str(), message.length(), 0);

        // Check if client wants to terminate
        if (message == "bye") {
            std::cout << "Client disconnected\n";
            break;
        }

        // Step 4: Receive data from the server
        memset(buffer, 0, sizeof(buffer));  // Clear the buffer
        read(sock, buffer, BUFFER_SIZE);
        std::cout << "Server: " << buffer << "\n";

        // Check if server sent "bye" to terminate
        if (strcmp(buffer, "bye") == 0) {
            std::cout << "Server disconnected\n";
            break;
        }
    }

    // Step 5: Close the socket
    close(sock);

    return 0;
}
