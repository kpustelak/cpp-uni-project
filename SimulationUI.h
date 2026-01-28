#ifndef SIMULATION_UI_H
#define SIMULATION_UI_H

#include "raylib.h"
#include "Run.h"

class SimulationUI {
private:
    int screenWidth;
    int screenHeight;
    int sidebarWidth;
    int gridAreaSize;

    int currentN;
    Run* simulationRun;

    bool paused;
    float timeAcc;
    float updateInterval;

    Color colHealthy;
    Color colInfected;
    Color colImmune;
    Color colBg;

    bool showPickerInfected;
    bool showPickerHealthy;
    bool showPickerImmune;

    int guiInfTime;
    int guiImmTime;
    int guiN;
    bool editModeN;
    bool bounds;

    // NOWE - tryb natychmiastowej zmiany czasu
    bool instantTimeChange;
    int pendingInfTime;  // Wartość oczekująca na zastosowanie
    int pendingImmTime;  // Wartość oczekująca na zastosowanie

    void Update();
    void Draw();
    void DrawSidebar();
    void DrawGrid(float cellSize);
    Color GetCellColor(Cell* cell);

public:
    SimulationUI(int width, int height);
    ~SimulationUI();
    void Execute();
};

#endif