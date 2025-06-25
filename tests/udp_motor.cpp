#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>

#include "../robot/base.h"

#define PORT_RECEIVE 40000
#define PORT_SEND 40001
#define BUFFER_SIZE 1024
#define TIMEOUT_SECONDS 2

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr, send_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    struct timeval last_receive_time, current_time, last_send_time;
    int timeout_printed = 0;
    int client_known = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set non-blocking mode
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT_RECEIVE);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Robot UDP server listening on port %d\n", PORT_RECEIVE);

    gettimeofday(&last_receive_time, NULL);
    gettimeofday(&last_send_time, NULL);

    float x_left = 0.0F;
    float y_left = 0.0F;
    float x_right = 0.0F;
    float y_right = 0.0F;

    float pre_x_left, pre_y_left;
    float pre_x_right, pre_y_right;

    bool changed = false;

    Odometry odo;

    Base base;
    base.readEncoders();
    odo = base.getOdometry();

    while (1) {
        ssize_t bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                                          (struct sockaddr *)&client_addr, &client_len);

        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            
            //printf("%s\n", buffer);
            pre_x_left = x_left;
            pre_y_left = y_left;
            pre_x_right = x_right;
            pre_y_right = y_right;
            sscanf(buffer, "%f %f %f %f", &x_left, &y_left, &x_right, &y_right);
            //printf("%f %f %f %f\n", x_left, y_left, x_right, y_right);

            gettimeofday(&last_receive_time, NULL);
            timeout_printed = 0;
            client_known = 1;  // now we know where to send responses

        } else if (bytes_received < 0) {
            if (errno != EWOULDBLOCK && errno != EAGAIN) {
                perror("recvfrom failed");
                break;
            }

            gettimeofday(&current_time, NULL);
            long elapsed_sec = current_time.tv_sec - last_receive_time.tv_sec;
            long elapsed_usec = current_time.tv_usec - last_receive_time.tv_usec;
            if (elapsed_usec < 0) {
                elapsed_sec--;
                elapsed_usec += 1000000;
            }

            if (elapsed_sec >= TIMEOUT_SECONDS && !timeout_printed) {
                printf("timeout\n");
                timeout_printed = 1;
                x_left = 0.0F;
                y_left = 0.0F;
                x_right = 0.0F;
                y_right = 0.0F;
            }
        }

        // Send system time every 100 ms
        gettimeofday(&current_time, NULL);
        long send_elapsed = (current_time.tv_sec*1000000 + current_time.tv_usec) - (last_send_time.tv_sec*1000000 + last_send_time.tv_usec);
        if (send_elapsed >= 100000 && client_known) {
            // Prepare client address for sending (port 40001)
            send_addr = client_addr;
            send_addr.sin_port = htons(PORT_SEND);

            char msg[BUFFER_SIZE];

            odo = base.getOdometry();

            snprintf(msg, BUFFER_SIZE, "x: %.4f, y: %.4f, ang: %.4f\n", odo.x, odo.y, odo.ang);

            sendto(sockfd, msg, strlen(msg), 0,
                   (struct sockaddr *)&send_addr, sizeof(send_addr));

            gettimeofday(&last_send_time, NULL);
        }

        base.readEncoders();

        changed = (pre_x_left != x_left)
                || (pre_y_left != y_left)
                || (pre_x_right != x_right)
                || (pre_y_right != y_right);

        if(changed)
        {
            base.setVelocity(x_left, y_left, -x_right);
            base.sendSpeed();
        }

        usleep(10);  // 10 Hz loop
    }

    close(sockfd);
    return 0;
}
