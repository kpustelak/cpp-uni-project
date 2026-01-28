#ifndef UNTITLED_RUN_H
#define UNTITLED_RUN_H

#include <vector>
#include <tuple>
#include <map>
#include "Board.h"

class SkinProblem;

class Run {
private:
    Board board;
    std::vector<SkinProblem*> problems;

    int settingsInfectionTime;
    int settingsImmunityTime;

    void rebuildCellProblemMap();
    std::map<Cell*, SkinProblem*> cellToProblemMap;

public:
    Run(int rows, int cols);
    ~Run();

    void updateGame();
    void spawnInfection(int r, int c);
    Board& getBoard();

    void setInfectionTime(int t);
    void setImmunityTime(int t);

    [[nodiscard]]int getInfectionTime() const;
    [[nodiscard]]int getImmunityTime() const;

    void setBoardBounds(bool enabled);
    [[nodiscard]]bool isBoundsEnabled() const;

    // NOWE METODY
    void resizeBoard(int newRows, int newCols);
    void updateExistingInfectionTimes();
    void updateExistingImmunityTimes();
    void resetBoard(); // RESET - wyczyść całą symulację

    // NOWA METODA - pobierz informacje o komórce dla gradientu
    // Zwraca: {isInfected, isImmune, remainingTime, maxTime}
    std::tuple<bool, bool, int, int> getCellProblemInfo(int row, int col) const;
};

#endif //UNTITLED_RUN_H