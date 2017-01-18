#include <cstdint>
#include <boost/log/trivial.hpp>
#include "Tcp.h"

/**
 * Constructor.
 * @param port_num port number.
 * @return Tcp object.
 */
Tcp::Tcp(uint16_t port_num) {
    port_number = port_num;
}

/**
 * Destructor.
 */
Tcp::~Tcp() {

}

/**
 * Initialize the socket.
 * @return int number representing the return status.
 */
int Tcp::initialize() {
    // Getting a socket descriptor and check if legal.
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor < 0) {
        // Return an error represent error at this method.
        return ERROR_SOCKET;
    }
    // Return correct if there were no problem.
    return CORRECT;
}

/**
 * Send the given data.
 * @param data what we need to send.
 * @param sockDescriptor -1 if the current object is a client-tcp-socket,
 * otherwise it will be number which mention the client socket descriptor.
 * @return
 */
int Tcp::sendData(string data, int sockDescriptor) {
    unsigned long data_len = data.size() + 1;
    const char *datas = data.c_str();
    // Check for socket descriptor, is server or client.
    if (sockDescriptor == -1) {
        sockDescriptor = socketDescriptor;
    }
    ssize_t sent_bytes = send(sockDescriptor, datas, data_len, 0);
    BOOST_LOG_TRIVIAL(debug) << "Sent msg: " << data << " from "
                             << sockDescriptor << " success: " << sent_bytes;
    if (sent_bytes < 0) {
        // Return an error represent error at this method.
        return ERROR_SEND;
    }
    // Return correct if there were no problem.
    return CORRECT;
}

/**
 * Getting data from the other socket and print the data.
 * @param buffer the place where the data will be stored.
 * @param size number.
 * @param sockDescriptor descriptor.
 * @return number.
 */
ssize_t Tcp::receiveData(char *buffer, size_t size, int sockDescriptor) {
    if (sockDescriptor == -1) {
        sockDescriptor = socketDescriptor;
    }
    ssize_t read_bytes = recv(sockDescriptor, buffer, size, 0);
    BOOST_LOG_TRIVIAL(debug) << "received msg: " << buffer << " bytes: "
                             << read_bytes;
    // Check the errors.
    if (read_bytes == 0) {
        return CONNECTION_CLOSED;
    } else if (read_bytes < 0) {
        // Return an error represent error at this method.
        return ERROR_RECEIVE;
    }
    // Return correct if there were no problem.
    return read_bytes;
}
