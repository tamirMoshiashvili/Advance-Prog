#include "TcpClient.h"

/**
 * Constructor.
 * @param port_num port number.
 * @return Tcp client socket object.
 */
TcpClient::TcpClient(uint16_t port_num) : Tcp(port_num) {

}

/**
 * Destructor.
 */
TcpClient::~TcpClient() {

}

/**
 * Initialize the server socket.
 * @return number representing return status.
 */
int TcpClient::initialize() {
    int returnVal = Tcp::initialize();
    if (returnVal != ERROR_SOCKET) {
        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = inet_addr((ip_address).c_str());
        sin.sin_port = htons(port_number);
        if (connect(socketDescriptor,
                    (struct sockaddr *) &sin, sizeof(sin)) < 0) {
            // Return an error represent error at this method.
            return ERROR_CONNECT;
        }
    }
    return returnVal;
}
