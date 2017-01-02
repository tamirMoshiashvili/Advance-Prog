#ifndef EX2_CLIENT_H
#define EX2_CLIENT_H


#include "Driver.h"

/**
 * Client is a driver that contact with some server,
 * and operate according to him.
 */
class Client : public Driver {
private:
    Socket *socket;

public:
    Client(int idNum, int ageVal, MaritalStatus status, int years, int cabID,
           const string &ip_addr, uint16_t port);

    ~Client();

    int sendData(string data);

    ssize_t receiveData(char *buffer, size_t size);

    void operate();

private:
    void sendIds();

    void connectToCab();

    void handleRide();

    void getRideFromServer();

    void sendLocationToServer();

    void handleNavigation();

    void drive();
};


#endif //EX2_CLIENT_H
