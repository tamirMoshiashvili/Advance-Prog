
#include "Driver/Client.h"
#include "Input/InputManager.h"
#include "Socket/Tcp/TcpClient.h"
#include <stdlib.h>

using namespace std;
using namespace boost;

int main(int argc, char **argv) {
    Socket *s = new TcpClient(argv[1], std::atoi(argv[2]));
    s->initialize();
    char buffer[128];
    s->receiveData(buffer, sizeof(buffer));
    cout << buffer;
    cin >> buffer;
    s->sendData(buffer);
    delete s;

    //Client *client = InputManager::readClient(argv[1], std::atoi(argv[2]));
    //client->operate();
    //delete client;
    return 0;











}
