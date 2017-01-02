#include "UdpClient.h"

/**
 * Constructor.
 * @param port_num port number.
 * @param ip_address ip address represented in string.
 * @return UdpClient object.
 */
UdpClient::UdpClient(std::string ip_addr, uint16_t port_num) : Udp(port_num) {
    ip_address = ip_addr;
}

/**
 * Destructor.
 */
UdpClient::~UdpClient() {

}
