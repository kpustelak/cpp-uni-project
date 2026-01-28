//
// Created by kornel on 12/9/25.
//

#include "SkinProblem.h"

void SkinProblem::addCell(Cell* cell){
    cells.push_back(cell);
}

bool SkinProblem::tick(){
    time -= 1;
    return time > 0;
}

SkinProblem::SkinProblem(Cell* originCell, int time): time(time){
    if(originCell){
        cells.push_back(originCell);
    }
}

std::vector<Cell*>& SkinProblem::getCells(){
    return cells;
}

/*Statyczne ustawienie czasu*/
void SkinProblem::setTime(int newTime) {
    time = newTime;
}

int SkinProblem::getTime() const {
    return time;
}