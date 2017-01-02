#ifndef EX2_RECOGNIZEABLE_H
#define EX2_RECOGNIZEABLE_H

#include <boost/serialization/access.hpp>

/**
 * Represents an object that can be recognized by its id.
 */
class Recognizable {
private:
    int id;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & id;
    }

public:
    Recognizable(int idNum);

    Recognizable();

    virtual ~Recognizable();

    int getId();

};


#endif //EX2_RECOGNIZEABLE_H
