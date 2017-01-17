#ifndef EX2_NAVIGATION_H
#define EX2_NAVIGATION_H


#include <deque>
#include "../../Basic/Point.h"
#include "../../Basic/Block/Block.h"
#include <boost/serialization/deque.hpp>

using namespace std;

class Navigation {
private:
    deque<Point> *path;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & path;
    }

public:
    Navigation(deque<string> *blockPath);

    Navigation();

    ~Navigation();

    Point getNextLocation();

    bool hasNextLocation();

};


#endif //EX2_NAVIGATION_H
