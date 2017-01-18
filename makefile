all: client.out server.out

server_files = mainServer.o Block.o MatrixBlock.o Point.o Recognizable.o \
               WayPasser.o Cab.o LuxuryCab.o StandardCab.o \
               KilometersPassedTracker.o SatisfactionTracker.o Client.o \
               Driver.o TaxiCenter.o MainFlow.o InputManager.o CityMap.o \
               MovementListener.o MovementNotifier.o BFS.o PathCalculator.o \
               Passenger.o Ride.o Tcp.o TcpClient.o TcpServer.o Socket.o \
               Navigation.o ThreadManagement.o GlobalInfo.o

server.out: $(server_files)
	g++ -g -o server.out $(server_files) -lboost_serialization -pthread

client_files = mainClient.o Block.o MatrixBlock.o Point.o Recognizable.o \
               WayPasser.o Cab.o LuxuryCab.o StandardCab.o \
               KilometersPassedTracker.o SatisfactionTracker.o Client.o \
               Driver.o TaxiCenter.o MainFlow.o InputManager.o CityMap.o \
               MovementListener.o MovementNotifier.o BFS.o \
               PathCalculator.o Passenger.o Ride.o Tcp.o TcpClient.o \
               TcpServer.o Socket.o Navigation.o ThreadManagement.o GlobalInfo.o

client.out: $(client_files)
	g++ -g -o client.out $(client_files) -lboost_serialization -pthread

mainServer.o: Source/mainServer.cpp
	g++ -g  -c Source/mainServer.cpp

mainClient.o: Source/mainClient.cpp
	g++ -g  -c Source/mainClient.cpp

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

MovementListener.o: Source/Movement/MovementListener.cpp Source/Movement/MovementListener.h
	g++ -Wall -c Source/Movement/MovementListener.cpp

MovementNotifier.o: Source/Movement/MovementNotifier.cpp Source/Movement/MovementNotifier.h
	g++ -Wall -c Source/Movement/MovementNotifier.cpp

BFS.o: Source/Ride/Navigation/BFS.cpp Source/Ride/Navigation/BFS.h
	g++ -Wall -c Source/Ride/Navigation/BFS.cpp

PathCalculator.o: Source/Ride/Navigation/PathCalculator.cpp Source/Ride/Navigation/PathCalculator.h
	g++ -Wall -c Source/Ride/Navigation/PathCalculator.cpp

Navigation.o: Source/Ride/Navigation/Navigation.cpp Source/Ride/Navigation/Navigation.h
	g++ -Wall -c Source/Ride/Navigation/Navigation.cpp

Passenger.o: Source/Ride/Passenger.cpp Source/Ride/Passenger.h
	g++ -Wall -c Source/Ride/Passenger.cpp

Ride.o: Source/Ride/Ride.cpp Source/Ride/Ride.h
	g++ -Wall -c Source/Ride/Ride.cpp

Tcp.o: Source/Socket/Tcp/Tcp.cpp Source/Socket/Tcp/Tcp.h
	g++ -Wall -c Source/Socket/Tcp/Tcp.cpp

TcpClient.o: Source/Socket/Tcp/TcpClient.cpp Source/Socket/Tcp/TcpClient.h
	g++ -Wall -c Source/Socket/Tcp/TcpClient.cpp

TcpServer.o: Source/Socket/Tcp/TcpServer.cpp Source/Socket/Tcp/TcpServer.h
	g++ -Wall -c Source/Socket/Tcp/TcpServer.cpp

Socket.o: Source/Socket/Socket.cpp Source/Socket/Socket.h
	g++ -Wall -c Source/Socket/Socket.cpp

GlobalInfo.o: ThreadControl/GlobalInfo.cpp ThreadControl/GlobalInfo.h
	g++ -Wall -c ThreadControl/GlobalInfo.cpp

ThreadManagement.o: ThreadControl/ThreadManagement.cpp ThreadControl/ThreadManagement.h
	g++ -Wall -c ThreadControl/ThreadManagement.cpp

clean:
	rm -f *.o *.out
