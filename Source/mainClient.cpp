
#include "Driver/Client.h"
#include "Input/InputManager.h"
#include <stdlib.h>

using namespace std;
using namespace boost;


int main(int argc, char **argv) {
    Client *client = InputManager::readClient(argv[1], std::atoi(argv[2]));
    client->operate();
    delete client;
    return 0;
}
