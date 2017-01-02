cmake_minimum_required(VERSION 3.3)
project(EX2)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -lpthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#add_subdirectory(lib/googletest-master)
#include_directories(lib/googletest-master/googletest/include)
#include_directories(lib/googletest-master/googlemock/include)

set(SOURCE_FILES Source/Basic/Point.cpp Source/Basic/Point.h
        Source/Ride/Passenger.cpp Source/Ride/Passenger.h
        Source/Basic/Block/Block.cpp Source/Basic/Block/Block.h
        Source/Basic/Block/MatrixBlock.cpp Source/Basic/Block/MatrixBlock.h
        Source/Map/CityMap.cpp Source/Map/CityMap.h
        Source/Ride/Ride.cpp Source/Ride/Ride.h
        Source/Ride/Navigation/Navigation.cpp Source/Ride/Navigation/Navigation.h
        Source/Cab/Vehicle/Cab.cpp Source/Cab/Vehicle/Cab.h
        Source/Cab/Vehicle/StandardCab.cpp Source/Cab/Vehicle/StandardCab.h
        Source/Cab/Vehicle/LuxuryCab.cpp Source/Cab/Vehicle/LuxuryCab.h
        Source/Driver/Driver.cpp Source/Driver/Driver.h
        Source/Movement/MovementListener.cpp Source/Movement/MovementListener.h
        Source/Movement/MovementNotifier.cpp Source/Movement/MovementNotifier.h
        Source/Driver/Tracker/KilometersPassedTracker.cpp
        Source/Driver/Tracker/KilometersPassedTracker.h
        Source/Cab/TaxiCenter.cpp Source/Cab/TaxiCenter.h
        Source/Basic/Recognizable.cpp Source/Basic/Recognizable.h
        Source/Basic/WayPasser.cpp Source/Basic/WayPasser.h
        Source/Basic/Block/MatrixBlock.cpp Source/Basic/Block/MatrixBlock.h
        Source/Map/LocationDetector.cpp Source/Map/LocationDetector.h
        Source/Control/MainFlow.cpp Source/Control/MainFlow.h
        Source/Input/InputManager.cpp Source/Input/InputManager.h
        Source/Ride/Navigation/BFS.cpp Source/Ride/Navigation/BFS.h
        Source/Driver/Tracker/SatisfactionTracker.cpp
        Source/Driver/Tracker/SatisfactionTracker.h
        Source/Driver/Client.cpp Source/Driver/Client.h

        Source/Socket/Socket.cpp Source/Socket/Socket.h
        Source/Socket/Udp/Udp.cpp Source/Socket/Udp/Udp.h
        Source/Socket/Udp/UdpServer.cpp Source/Socket/Udp/UdpServer.h
        Source/Socket/Udp/UdpClient.cpp Source/Socket/Udp/UdpClient.h

        #Tests/PointTest.cpp
        #Tests/RecognizableTest.cpp
        #Tests/WayPasserTest.cpp
        #Tests/BlockTest.cpp
        #Tests/DriverTest.cpp
        #Tests/TaxiCenterTest.cpp
        #Tests/KilometersPassedTrackerTest.cpp
        #Tests/RideTest.cpp
        #Tests/PassengerTest.cpp
        #Tests/NavigationTest.cpp
        #Tests/MovementNotifierTest.cpp
        #Tests/CityMapTest.cpp
        #Tests/CabTest.cpp
        #Tests/SatisfactionTrackerTest.cpp
        #Tests/LocationDetectorTest.cpp
        #Tests/InputParserTest.cpp
        #Tests/MainFlowTest.cpp
        )

set(BOOST_ROOT CHACHE PATH "/usr/include/boost/")
find_package(Boost 1.58.0 COMPONENTS serialization system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(server ${SOURCE_FILES} mainServer.cpp)
add_executable(client ${SOURCE_FILES} mainClient.cpp)
target_link_libraries(server ${Boost_LIBRARIES})
target_link_libraries(client ${Boost_LIBRARIES})

#add_executable(EX2 ${SOURCE_FILES})
#target_link_libraries(EX2 gtest gtest_main)