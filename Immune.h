//
// Created by korne on 12/11/2025.
//

#ifndef UNTITLED_IMMUNE_H
#define UNTITLED_IMMUNE_H
#include "SkinProblem.h"


class SkinProblem;
class Cell;
/*Klasa służy jedynie odróżnieniu infected od immunie i stworzeniu */
class Immune: public SkinProblem {
public:
    Immune(Cell* cell, int infectionTime): SkinProblem(cell, infectionTime){}
    void applyEffect(Board &board) override {}
};


#endif //UNTITLED_IMMUNE_H