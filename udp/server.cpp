#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 12345  // UDP port number
#define BUF_SIZE 65507  // Max UDP packet size (64 KB)

int main() {
    // Create UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    // Set up the server address
    struct sockaddr_in serverAddr, clientAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    char buffer[BUF_SIZE];
    socklen_t addr_len = sizeof(clientAddr);

    while (true) {
        // Receive the video frame data
        int len = recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr*)&clientAddr, &addr_len);
        if (len > 0) {
            // Convert the received data into a Mat (OpenCV format)
            cv::Mat img = cv::imdecode(cv::Mat(1, len, CV_8U, buffer), cv::IMREAD_COLOR);
            if (img.empty()) {
                std::cerr << "Failed to decode the frame" << std::endl;
                continue;
            }

            // Display the received frame
            cv::imshow("Received Video", img);
            if (cv::waitKey(1) == 27)  // Exit on ESC key
                break;
        }
    }

    close(sockfd);
    return 0;
}
