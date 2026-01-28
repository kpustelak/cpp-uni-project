//
// Created by kornel on 12/9/25.
//

#ifndef UNTITLED_SKINPROBLEM_H
#define UNTITLED_SKINPROBLEM_H

#include <vector>
class Board;
class Cell;

class SkinProblem {
protected:
    std::vector<Cell*> cells;
    int time;
public:
    SkinProblem(Cell* originCell, int time) ;

    virtual ~SkinProblem() = default;

    void addCell(Cell* cell);

    std::vector<Cell*>& getCells();

    bool tick();

    virtual void applyEffect(Board &board) =0;

    void setTime(int newTime);
    int getTime() const;
};


#endif //UNTITLED_SKINPROBLEM_H