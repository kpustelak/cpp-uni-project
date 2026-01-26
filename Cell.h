//
// Created by kornel on 12/9/25.
//

#ifndef UNTITLED_CELL_H
#define UNTITLED_CELL_H

enum CellState {EMPTY ,HEALTHY, IMMUNE, INFECTED};

class Cell {
private:
    int x,y;
    CellState currentState;
    CellState nextState;
public:
    Cell(): x(0), y(0), currentState(HEALTHY), nextState(EMPTY) {}

    void setPos(int newX, int newY);
    [[nodiscard]] int getX()const ;
    [[nodiscard]] int getY()const ;

    [[nodiscard]] CellState getCurrentState()const;

    void swapNextStateToCurrent();

    void setNextState(CellState newNextState);
    [[nodiscard]] bool haveNextState() const;
    void cleanNextState();
};


#endif //UNTITLED_CELL_H