//
// Created by kornel on 12/9/25.
//

#include "Infected.h"
#include "Board.h"
#include <cstdlib>

void Infected::applyEffect(Board &board){
    for(auto cell : cells){
        std::vector<Cell *> cellsToCheck = board.getCellsAround(cell->getX(), cell->getY());
        for(auto cellToCheck: cellsToCheck){
            if(cellToCheck->getCurrentState() == HEALTHY && cellToCheck->haveNextState() == false){
                if((rand() % 100) < 50){
                    cellToCheck->setNextState(INFECTED);
                }
            }
        }
    }
}