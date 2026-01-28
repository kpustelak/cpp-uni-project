#include "Run.h"
#include "Infected.h"
#include "Immune.h"
#include "SkinProblem.h"
#include <tuple>
#include <algorithm>
#include <cmath>

Run::Run(int rows, int cols)
    : board(rows, cols, true),
      settingsInfectionTime(5),
      settingsImmunityTime(10)
{
}

Run::~Run() {
    for (auto p : problems) delete p;
    problems.clear();
}

void Run::spawnInfection(int r, int c) {
    if (r < 0 || r >= board.getRows() || c < 0 || c >= board.getCols()) return;

    Cell* cell = board.getCell(r, c);
    if(cell == nullptr) return;

    CellState currentState = cell->getCurrentState();

    if (currentState == INFECTED) {
        return;
    }

    if (currentState == IMMUNE) {
        auto it = cellToProblemMap.find(cell);
        if (it != cellToProblemMap.end()) {
            SkinProblem* immuneProblem = it->second;


            std::vector<Cell*>& cells = immuneProblem->getCells();
            /*Przyspieszenie zamiast przesuwania to na koniec i usuwamy*/
            cells.erase(std::remove(cells.begin(), cells.end(), cell), cells.end());

            cellToProblemMap.erase(it);

            if (cells.empty()) {
                problems.erase(std::remove(problems.begin(), problems.end(), immuneProblem), problems.end());
                delete immuneProblem;
            }
        }
    }

    cell->setNextState(INFECTED);
    cell->swapNextStateToCurrent();
    cell->cleanNextState();

    problems.push_back(new Infected(cell, settingsInfectionTime));
    cellToProblemMap[cell] = problems.back();
}

void Run::updateGame() {
    std::vector<SkinProblem*> remainingProblems;
    std::vector<SkinProblem*> newProblems;

    for (auto problem : problems) {
        problem->applyEffect(board);

        if (problem->tick()) {
            remainingProblems.push_back(problem);
        }
        else {
            std::vector<Cell*>& cells = problem->getCells();

            if (dynamic_cast<Infected*>(problem)) {
                for (Cell* c : cells) {
                    c->setNextState(IMMUNE);
                }
            }
            else if (dynamic_cast<Immune*>(problem)) {
                for (Cell* c : cells) {
                    c->setNextState(HEALTHY);
                }
            }
            delete problem;
        }
    }

    int rows = board.getRows();
    int cols = board.getCols();

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            Cell* cell = board.getCell(r, c);

            if (cell->haveNextState()) {
                if (cell->getCurrentState() == HEALTHY) {
                    newProblems.push_back(new Infected(cell, settingsInfectionTime));
                }
                else if (cell->getCurrentState() == INFECTED) {
                    newProblems.push_back(new Immune(cell, settingsImmunityTime));
                }

                cell->swapNextStateToCurrent();
                cell->cleanNextState();
            }
        }
    }

    problems = remainingProblems;
    problems.insert(problems.end(), newProblems.begin(), newProblems.end());
    rebuildCellProblemMap();
}

Board& Run::getBoard(){ return board; }

void Run::setInfectionTime(const int t) {
    settingsInfectionTime = (t > 1) ? t : 1;
    updateExistingInfectionTimes();
}

void Run::setImmunityTime(const int t) {
    settingsImmunityTime = (t > 1) ? t : 1;
    updateExistingImmunityTimes();
}

int Run::getInfectionTime() const { return settingsInfectionTime; }

int Run::getImmunityTime() const { return settingsImmunityTime; }

void Run::setBoardBounds(bool enabled) { board.setBoundsEnabled(enabled); }

bool Run::isBoundsEnabled() const { return board.isBoundsEnabled(); }

void Run::resetBoard() {
    for (auto problem : problems) {
        delete problem;
    }
    problems.clear();
    cellToProblemMap.clear();

    int rows = board.getRows();
    int cols = board.getCols();

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Cell* cell = board.getCell(r, c);
            if (cell != nullptr) {
                cell->setNextState(HEALTHY);
                cell->swapNextStateToCurrent();
                cell->cleanNextState();
            }
        }
    }
}


void Run::resizeBoard(int newRows, int newCols) {

    std::vector<std::tuple<int, int, CellState, bool, int>> cellStates;
    for (auto problem : problems) {
        if(problem == nullptr) continue;

        std::vector<Cell*>& cells = problem->getCells();
        int remainingTime = problem->getTime();

        for (Cell* cell : cells) {
            if(cell != nullptr) {
                int x = cell->getX();
                int y = cell->getY();
                CellState state = cell->getCurrentState();
                bool isInfected = (dynamic_cast<Infected*>(problem) != nullptr);

                if(x >= 0 && x < newRows && y >= 0 && y < newCols) {
                    cellStates.push_back(std::make_tuple(x, y, state, isInfected, remainingTime));
                }
            }
        }
    }

    for (auto problem : problems) {
        delete problem;
    }
    problems.clear();

    board.resize(newRows, newCols);

    for (const auto& cellInfo : cellStates) {
        int x = std::get<0>(cellInfo);
        int y = std::get<1>(cellInfo);
        CellState state = std::get<2>(cellInfo);
        bool isInfected = std::get<3>(cellInfo);
        int remainingTime = std::get<4>(cellInfo);

        Cell* newCell = board.getCell(x, y);
        if(newCell == nullptr) continue;

        newCell->setNextState(state);
        newCell->swapNextStateToCurrent();
        newCell->cleanNextState();

        if(isInfected) {
            SkinProblem* newProblem = new Infected(newCell, remainingTime);
            problems.push_back(newProblem);
        } else {
            SkinProblem* newProblem = new Immune(newCell, remainingTime);
            problems.push_back(newProblem);
        }
    }

    rebuildCellProblemMap();
}

void Run::updateExistingInfectionTimes() {
    for (auto problem : problems) {
        if (dynamic_cast<Infected*>(problem)) {
            int currentTime = problem->getTime();

            if (currentTime >= 1) {
                problem->setTime(settingsInfectionTime);
            }
        }
    }
}

void Run::updateExistingImmunityTimes() {
    for (auto problem : problems) {
        if (dynamic_cast<Immune*>(problem)) {
            int currentTime = problem->getTime();

            if (currentTime > 0) {
                problem->setTime(settingsImmunityTime);
            }
        }
    }
}

void Run::rebuildCellProblemMap() {
    cellToProblemMap.clear();

    for (auto problem : problems) {
        if (problem == nullptr) continue;

        std::vector<Cell*>& cells = problem->getCells();
        for (Cell* cell : cells) {
            if (cell != nullptr) {
                cellToProblemMap[cell] = problem;
            }
        }
    }
}

std::tuple<bool, bool, int, int> Run::getCellProblemInfo(int row, int col) const {
    const Cell* cell = board.getCell(row, col);
    if (cell == nullptr) {
        return std::make_tuple(false, false, 0, 0);
    }

    auto it = cellToProblemMap.find(const_cast<Cell*>(cell));
    if (it != cellToProblemMap.end()) {
        SkinProblem* problem = it->second;

        bool isInfected = (dynamic_cast<Infected*>(problem) != nullptr);
        bool isImmune = (dynamic_cast<Immune*>(problem) != nullptr);
        int remainingTime = problem->getTime();
        int maxTime = isInfected ? settingsInfectionTime : settingsImmunityTime;

        return std::make_tuple(isInfected, isImmune, remainingTime, maxTime);
    }

    return std::make_tuple(false, false, 0, 0);
}