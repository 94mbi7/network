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
#define SERVER_IP "127.0.0.1"  // Server IP address (local in this case)
#define BUF_SIZE 65507  // Max UDP packet size (64 KB)

int main() {
    // Create UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    // Set up the server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Open the webcam
    cv::VideoCapture cap(0);  // Open the default camera
    if (!cap.isOpened()) {
        std::cerr << "Error opening video stream" << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (true) {
        // Capture a frame
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Failed to capture frame" << std::endl;
            break;
        }

        // Encode the frame into a buffer (JPEG format)
        std::vector<uchar> buf;
        cv::imencode(".jpg", frame, buf);

        // Send the encoded frame over UDP
        int len = buf.size();
        if (sendto(sockfd, buf.data(), len, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
            std::cerr << "Failed to send frame" << std::endl;
            break;
        }

        // Wait briefly to simulate real-time streaming
        cv::waitKey(1);
    }

    // Release the webcam and close the socket
    cap.release();
    close(sockfd);
    return 0;
}
