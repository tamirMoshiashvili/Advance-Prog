#include "Udp.h"

/**
 * Constructor.
 * @param port_num port number.
 * @return Udp object.
 */
Udp::Udp(uint16_t port_num) : Socket() {
    port_number = port_num;
}

/**
 * Destructor.
 */
Udp::~Udp() {

}

/**
 * Initialize the Socket object and getting a socket descriptor.
 * @return int number representing the return status.
 */
int Udp::initialize() {
    // Creating new socket and getting his descriptor.
    socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketDescriptor < 0) {
        return ERROR_SOCKET;
    }
    // Return correct if there were no problems.
    return CORRECT;
}

/**
 * Sending the input data to the socket who connect to this socket.
 * @param data string representing the data to send.
 * @param sockDescriptor descriptor.
 * @return number representing the return status.
 */
int Udp::sendData(string data, int sockDescriptor) {
    // Initialize the struct.
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip_address.c_str());
    sin.sin_port = htons(port_number);
    const char *datas = data.c_str();
    unsigned long data_len = data.size() + 1;
    // Send.
    ssize_t sent_bytes = sendto(socketDescriptor, datas, data_len, 0,
                                (struct sockaddr *) &sin, sizeof(sin));
    // Check if send successfully.
    if (sent_bytes < 0) {
        return ERROR_SEND;
    }
    // Return correct if there were no problems.
    return CORRECT;
}

/**
* Getting data from the other socket and print the data.
* @param buffer the place where the data will be stored.
* @param size number.
* @return number of bytes received.
*/
ssize_t Udp::receiveData(char *buffer, size_t size, int sockDescriptor) {
    struct sockaddr_in to;
    unsigned int to_len = sizeof(struct sockaddr_in);
    // Receive.
    ssize_t bytes = recvfrom(socketDescriptor, buffer, size, 0,
                             (struct sockaddr *) &to, &to_len);
    // Set the port number to the new one which we get with the data.
    port_number = ntohs(to.sin_port);
    // Check if receive successfully.
    if (bytes < 0) {
        return -1;
    }
    // Return correct if there were no error.
    return bytes;
}
