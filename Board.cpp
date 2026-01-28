#include "Board.h"

Board::Board(int rows, int columns, bool boundsEnabled) 
    : rows(rows), columns(columns), boundsEnabled(boundsEnabled) {
    grid.resize(rows);
    for(int i = 0; i < rows; i++) {
        grid[i].resize(columns);
        for(int j = 0; j < columns; j++) {
            grid[i][j].setPos(i, j);
        }
    }
}

void Board::setBoundsEnabled(bool enabled) {
    boundsEnabled = enabled;
}

bool Board::isBoundsEnabled() const {
    return boundsEnabled;
}

Cell* Board::getCell(int row, int col) {
    if(row < 0 || row >= rows || col < 0 || col >= columns) {
        return nullptr;
    }
    return &grid[row][col];
}

const Cell* Board::getCell(int row, int col) const {
    if(row < 0 || row >= rows || col < 0 || col >= columns) {
        return nullptr;
    }
    return &grid[row][col];
}

std::vector<Cell*> Board::getCellsAround(int row, int col) {
    std::vector<Cell*> neighbors;

    for(int dr = -1; dr <= 1; dr++) {
        for(int dc = -1; dc <= 1; dc++) {
            if(dr == 0 && dc == 0) continue;

            int newRow = row + dr;
            int newCol = col + dc;

            if(boundsEnabled) {
                if(newRow >= 0 && newRow < rows && newCol >= 0 && newCol < columns) {
                    neighbors.push_back(&grid[newRow][newCol]);
                }
            } else {
                newRow = (newRow + rows) % rows;
                newCol = (newCol + columns) % columns;
                neighbors.push_back(&grid[newRow][newCol]);
            }
        }
    }

    return neighbors;
}

int Board::getRows() {
    return rows;
}

int Board::getCols() {
    return columns;
}

/*Zmiana rozmiaru bez resetowania*/
void Board::resize(int newRows, int newCols) {
    if(newRows == rows && newCols == columns) {
        return;
    }

    if(newRows <= 0 || newCols <= 0) {
        return;
    }

    /*Tworzenie nowej siatki wedle wskazówek z ostatnich konsultacji*/
    std::vector<std::vector<Cell>> newGrid;
    newGrid.resize(newRows);
    for(int i = 0; i < newRows; i++) {
        newGrid[i].resize(newCols);
        for(int j = 0; j < newCols; j++) {
            newGrid[i][j].setPos(i, j);
        }
    }

    int copyRows = std::min(rows, newRows);
    int copyCols = std::min(columns, newCols);

    /*Przekopiowanie wartości po minimum do kolejne tablicy*/
    for(int i = 0; i < copyRows; i++) {
        for(int j = 0; j < copyCols; j++) {
            if(i >= 0 && i < rows && j >= 0 && j < columns &&
               i >= 0 && i < newRows && j >= 0 && j < newCols) {
                newGrid[i][j] = grid[i][j];
                newGrid[i][j].setPos(i, j);
            }
        }
    }


    rows = newRows;
    columns = newCols;
    grid = std::move(newGrid);
}