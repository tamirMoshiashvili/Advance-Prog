#include "TcpServer.h"

/**
 * Constructor.
 * @param ip_addr ip address.
 * @param port_num port number.
 * @param numOfClients number of clients.
 * @return Tcp server socket object.
 */
TcpServer::TcpServer(string ip_addr, uint16_t port_num, int numOfClients)
        : Tcp(port_num), numClients(numOfClients) {
    ip_address = ip_addr;
    clientDescriptors = new vector<int>();
}

/**
 * Destructor.
 */
TcpServer::~TcpServer() {
    delete clientDescriptors;
}

/**
 * Initialize the server socket.
 * @return number representing return status.
 */
int TcpServer::initialize() {
    int returnVal = Tcp::initialize();
    if (returnVal != ERROR_SOCKET) {
        // Initialize the struct.
        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_port = htons(port_number);
        // Bind.
        if (bind(socketDescriptor, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
            // Return an error represent error at this method.
            return ERROR_BIND;
        }
        // Listen.
        if (listen(socketDescriptor, backLog) < 0) {
            // Return an error represent error at this method.
            return ERROR_LISTEN;
        }
        // Accept.
        int clientDescriptor = 0;
        unsigned int addr_len = 0;
        for (int i = 0; i < numClients; ++i) {
            struct sockaddr_in client_sin;
            addr_len = sizeof(client_sin);
            clientDescriptor = accept(socketDescriptor,
                                      (struct sockaddr *) &client_sin,
                                      &addr_len);
            clientDescriptors->push_back(clientDescriptor);
            if (clientDescriptor < 0) {
                // Return an error represent error at this method.
                return ERROR_ACCEPT;
            }
        }
    }
    return returnVal;
}
