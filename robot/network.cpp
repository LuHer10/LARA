#include "network.h"

ssize_t Network::receive(const char *buf)
{
    ssize_t bytes_received = recvfrom(sockfd, (char*)buf, BUFFER_SIZE - 1, 0,
                                          (struct sockaddr *)&client_addr, &client_len);
    
    
    /*if (bytes_received < 0) {
        if (errno != EWOULDBLOCK && errno != EAGAIN) {
            perror("recvfrom failed");
        }
    }*/
    
    return bytes_received;
}

void Network::receiveJoysticks(float &x_l, float &y_l, float &x_r, float &y_r)
{
    ssize_t bytes_received = receive(buffer);

    if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            
            //printf("%s\n", buffer);
            sscanf(buffer, "%f %f %f %f", &x_l, &y_l, &x_r, &y_r);
            //printf("%f %f %f %f\n", x_left, y_left, x_right, y_right);

            gettimeofday(&last_receive_time, NULL);
            timeout_printed = 0;
            client_known = 1;  // now we know where to send responses

        } else if (bytes_received < 0) {
            if (errno != EWOULDBLOCK && errno != EAGAIN) {
                perror("recvfrom failed");
                return;
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
                x_l = 0.0F;
                y_l = 0.0F;
                x_r = 0.0F;
                y_r = 0.0F;
            }
        }
}

void Network::send(const char *buf)
{
    sendto(sockfd, buf, strlen(buf), 0,
                   (struct sockaddr *)&send_addr, sizeof(send_addr));

}

void Network::sendOdometry(float x, float y, float ang, long dt_us)
{
    // Send system time every 100 ms
        gettimeofday(&current_time, NULL);
        long send_elapsed = (current_time.tv_sec*1000000 + current_time.tv_usec) - (last_send_time.tv_sec*1000000 + last_send_time.tv_usec);
        if (send_elapsed >= 100000 && client_known) {
            // Prepare client address for sending (port 40001)
            send_addr = client_addr;
            send_addr.sin_port = htons(PORT_SEND);

            char msg[BUFFER_SIZE];

            snprintf(msg, BUFFER_SIZE, "x: %.4f, y: %.4f, ang: %.4f\n", x, y, ang);

            sendto(sockfd, msg, strlen(msg), 0,
                   (struct sockaddr *)&send_addr, sizeof(send_addr));

            gettimeofday(&last_send_time, NULL);
        }
}