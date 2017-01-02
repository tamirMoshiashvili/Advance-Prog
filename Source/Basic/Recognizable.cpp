#include "Recognizable.h"

/**
 * Constructor.
 * @param idNum id number.
 * @return Recognizable object.
 */
Recognizable::Recognizable(int idNum) : id(idNum) {

}

/**
 * Default Constructor.
 * @return Recognizable object.
 */
Recognizable::Recognizable() : id(0) {

}

/**
 * Destructor.
 */
Recognizable::~Recognizable() {

}

/**
 * Get the id number of the object.
 * @return id number.
 */
int Recognizable::getId() {
    return id;
}
