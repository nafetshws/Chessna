#include <string>
#include "../include/types.hpp"

#ifndef MOVE_H
#define MOVE_H


struct Move {

    Move(Square _origin, Square _destination) : origin(_origin), destination(_destination) {}

    Square origin;
    Square destination;
};

#endif