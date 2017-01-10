
#include "Driver/Client.h"
#include "Input/InputManager.h"
#include "Socket/Tcp/TcpClient.h"
#include <stdlib.h>

using namespace std;
using namespace boost;

int main(int argc, char **argv) {
    Socket *socket = new TcpClient(argv[1], std::atoi(argv[2]));
    socket->initialize();
    socket->sendData("hello\n");
    char buffer[32] = {0};
    socket->receiveData(buffer, sizeof(buffer));
    string str(buffer);
    cout << str;

    delete socket;
//    Client *client = InputManager::readClient(argv[1], std::atoi(argv[2]));
//    client->operate();
//    delete client;
    return 0;
}
