#include "UdpServer.h"

/**
 * Constructor.
 * @param port_num port number.
 * @return UdpServer object.
 */
UdpServer::UdpServer(uint16_t port_num) : Udp(port_num) {

}

/**
 * Destructor.
 */
UdpServer::~UdpServer() {

}

/**
 * Initialize the Socket object and getting a socket descriptor.
 * @return int number representing the return status.
 */
int UdpServer::initialize() {
    int returnVal = Udp::initialize();
    if (returnVal != ERROR_SOCKET) {
        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_port = htons(port_number);
        // Bind.
        if (bind(socketDescriptor, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
            return ERROR_BIND;
        }
    }
    return returnVal;
}
