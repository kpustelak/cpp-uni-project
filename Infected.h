//
// Created by kornel on 12/9/25.
//

#ifndef UNTITLED_INFECTED_H
#define UNTITLED_INFECTED_H

#include "SkinProblem.h"
#include "Board.h"
#include "Cell.h"


class Infected : public SkinProblem {
public:

    Infected(Cell* cell, int infectionTime): SkinProblem(cell, infectionTime){}

    /*Zarażenie komórek wokoł*/
    void applyEffect(Board &board) override;
};

#endif //UNTITLED_INFECTED_H