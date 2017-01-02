#include "Socket.h"
#include <unistd.h>

#define NONE 0

/**
 * Constructor.
 * @return Socket object.
 */
Socket::Socket() {
    ip_address = IP;
    backLog = NONE;
    socketDescriptor = NONE;
    port_number = NONE;
}

/**
 * Destructor.
 */
Socket::~Socket() {
    close(socketDescriptor);
}
