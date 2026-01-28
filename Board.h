#ifndef UNTITLED_BOARD_H
#define UNTITLED_BOARD_H

#include <vector>
#include "Cell.h"

class Board {
private:
    int rows;
    int columns;
    bool boundsEnabled;
    std::vector<std::vector<Cell>> grid;

public:
    Board(int rows, int columns, bool boundsEnabled);

    void setBoundsEnabled(bool enabled);
    bool isBoundsEnabled() const;

    Cell* getCell(int row, int col);
    const Cell* getCell(int row, int col) const;
    std::vector<Cell *> getCellsAround(int row, int col);

    int getRows();
    int getCols();

    // Zmiana rozmiaru bez resetowania
    void resize(int newRows, int newCols);
};

#endif //UNTITLED_BOARD_H