#include "Cell.h"

void Cell::setPos(int newX, int newY) {
    x = newX;
    y = newY;
}

int Cell::getX() const {
    return x;
}

int Cell::getY() const {
    return y;
}

CellState Cell::getCurrentState() const {
    return currentState;
}

void Cell::swapNextStateToCurrent() {
    if(nextState != EMPTY) {
        currentState = nextState;
        nextState = EMPTY;
    }
}

void Cell::setNextState(CellState newNextState) {
    nextState = newNextState;
}

bool Cell::haveNextState() const {
    return nextState != EMPTY;
}

void Cell::cleanNextState() {
    nextState = EMPTY;
}