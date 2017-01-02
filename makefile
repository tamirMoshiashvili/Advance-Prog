all: server.out client.out

server.out: mainServer.o Block.o MatrixBlock.o Point.o Recognizable.o WayPasser.o Cab.o LuxuryCab.o StandardCab.o KilometersPassedTracker.o SatisfactionTracker.o Client.o Driver.o TaxiCenter.o MainFlow.o InputManager.o CityMap.o LocationDetector.o  MovementListener.o MovementNotifier.o BFS.o Navigation.o Passenger.o Ride.o Udp.o UdpClient.o UdpServer.o Socket.o
	g++ -g -o server.out mainServer.o Block.o MatrixBlock.o Point.o Recognizable.o WayPasser.o Cab.o LuxuryCab.o StandardCab.o KilometersPassedTracker.o SatisfactionTracker.o Client.o Driver.o TaxiCenter.o MainFlow.o InputManager.o CityMap.o LocationDetector.o  MovementListener.o MovementNotifier.o BFS.o Navigation.o Passenger.o Ride.o Udp.o UdpClient.o UdpServer.o Socket.o -lboost_serialization

client.out: mainClient.o Block.o MatrixBlock.o Point.o Recognizable.o WayPasser.o Cab.o LuxuryCab.o StandardCab.o KilometersPassedTracker.o SatisfactionTracker.o Client.o Driver.o TaxiCenter.o MainFlow.o InputManager.o CityMap.o LocationDetector.o MovementListener.o MovementNotifier.o BFS.o Navigation.o Passenger.o Ride.o Udp.o UdpClient.o UdpServer.o Socket.o
	g++ -g -o client.out mainClient.o Block.o MatrixBlock.o Point.o Recognizable.o WayPasser.o Cab.o LuxuryCab.o StandardCab.o KilometersPassedTracker.o SatisfactionTracker.o Client.o Driver.o TaxiCenter.o MainFlow.o InputManager.o CityMap.o LocationDetector.o MovementListener.o MovementNotifier.o BFS.o Navigation.o Passenger.o Ride.o Udp.o UdpClient.o UdpServer.o Socket.o -lboost_serialization

mainServer.o: mainServer.cpp
	g++ -g  -c mainServer.cpp

mainClient.o: mainClient.cpp
	g++ -g  -c mainClient.cpp

Block.o: Source/Basic/Block/Block.cpp Source/Basic/Block/Block.h
	g++ -Wall -c Source/Basic/Block/Block.cpp

MatrixBlock.o: Source/Basic/Block/MatrixBlock.cpp Source/Basic/Block/MatrixBlock.h
	g++ -Wall -c Source/Basic/Block/MatrixBlock.cpp

Point.o: Source/Basic/Point.cpp Source/Basic/Point.h
	g++ -Wall -c Source/Basic/Point.cpp

Recognizable.o: Source/Basic/Recognizable.cpp Source/Basic/Recognizable.h
	g++ -Wall -c Source/Basic/Recognizable.cpp

WayPasser.o: Source/Basic/WayPasser.cpp Source/Basic/WayPasser.h
	g++ -Wall -c Source/Basic/WayPasser.cpp

Cab.o: Source/Cab/Vehicle/Cab.cpp Source/Cab/Vehicle/Cab.h
	g++ -Wall -c Source/Cab/Vehicle/Cab.cpp

LuxuryCab.o: Source/Cab/Vehicle/LuxuryCab.cpp Source/Cab/Vehicle/LuxuryCab.h
	g++ -Wall -c Source/Cab/Vehicle/LuxuryCab.cpp

StandardCab.o: Source/Cab/Vehicle/StandardCab.cpp Source/Cab/Vehicle/StandardCab.h
	g++ -Wall -c Source/Cab/Vehicle/StandardCab.cpp

TaxiCenter.o: Source/Cab/TaxiCenter.cpp Source/Cab/TaxiCenter.h
	g++ -Wall -c Source/Cab/TaxiCenter.cpp

MainFlow.o: Source/Control/MainFlow.cpp Source/Control/MainFlow.h
	g++ -Wall -c Source/Control/MainFlow.cpp

KilometersPassedTracker.o: Source/Driver/Tracker/KilometersPassedTracker.cpp Source/Driver/Tracker/KilometersPassedTracker.h
	g++ -Wall -c Source/Driver/Tracker/KilometersPassedTracker.cpp

SatisfactionTracker.o: Source/Driver/Tracker/SatisfactionTracker.cpp Source/Driver/Tracker/SatisfactionTracker.h
	g++ -Wall -c Source/Driver/Tracker/SatisfactionTracker.cpp

Client.o: Source/Driver/Client.cpp Source/Driver/Client.h
	g++ -std=c++0x -Wall -c Source/Driver/Client.cpp

Driver.o: Source/Driver/Driver.cpp Source/Driver/Driver.h
	g++ -Wall -c Source/Driver/Driver.cpp

InputManager.o: Source/Input/InputManager.cpp Source/Input/InputManager.h
	g++ -Wall -c Source/Input/InputManager.cpp

CityMap.o: Source/Map/CityMap.cpp Source/Map/CityMap.h
	g++ -Wall -c Source/Map/CityMap.cpp

LocationDetector.o: Source/Map/LocationDetector.cpp Source/Map/LocationDetector.h
	g++ -Wall -c Source/Map/LocationDetector.cpp

MovementListener.o: Source/Movement/MovementListener.cpp Source/Movement/MovementListener.h
	g++ -Wall -c Source/Movement/MovementListener.cpp

MovementNotifier.o: Source/Movement/MovementNotifier.cpp Source/Movement/MovementNotifier.h
	g++ -Wall -c Source/Movement/MovementNotifier.cpp

BFS.o: Source/Ride/Navigation/BFS.cpp Source/Ride/Navigation/BFS.h
	g++ -Wall -c Source/Ride/Navigation/BFS.cpp

Navigation.o: Source/Ride/Navigation/Navigation.cpp Source/Ride/Navigation/Navigation.h
	g++ -Wall -c Source/Ride/Navigation/Navigation.cpp

Passenger.o: Source/Ride/Passenger.cpp Source/Ride/Passenger.h
	g++ -Wall -c Source/Ride/Passenger.cpp

Ride.o: Source/Ride/Ride.cpp Source/Ride/Ride.h
	g++ -Wall -c Source/Ride/Ride.cpp

Udp.o: Source/Socket/Udp/Udp.cpp Source/Socket/Udp/Udp.h
	g++ -Wall -c Source/Socket/Udp/Udp.cpp

UdpClient.o: Source/Socket/Udp/UdpClient.cpp Source/Socket/Udp/UdpClient.h
	g++ -Wall -c Source/Socket/Udp/UdpClient.cpp

UdpServer.o: Source/Socket/Udp/UdpServer.cpp Source/Socket/Udp/UdpServer.h
	g++ -Wall -c Source/Socket/Udp/UdpServer.cpp

Socket.o: Source/Socket/Socket.cpp Source/Socket/Socket.h
	g++ -Wall -c Source/Socket/Socket.cpp

clean:
	rm -f *.o *.out
