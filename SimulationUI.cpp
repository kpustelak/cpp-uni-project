#include "SimulationUI.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

SimulationUI::SimulationUI(int width, int height) {
    screenWidth = width;
    screenHeight = height;
    sidebarWidth = 400;
    gridAreaSize = height;

    currentN = 50;
    simulationRun = new Run(currentN, currentN);

    paused = true;
    timeAcc = 0.0f;
    updateInterval = 0.1f;

    colHealthy = LIGHTGRAY;
    colInfected = RED;
    colImmune = BLUE;
    colBg = { 40, 40, 40, 255 };

    showPickerInfected = false;
    showPickerHealthy = false;
    showPickerImmune = false;

    guiInfTime = simulationRun->getInfectionTime();
    guiImmTime = simulationRun->getImmunityTime();
    guiN = currentN;
    editModeN = false;
    bounds = simulationRun->isBoundsEnabled();

    InitWindow(screenWidth, screenHeight, "LISZAJ SYMULACJA - Kornel Pustelak - 179599");
    SetTargetFPS(60);
}

SimulationUI::~SimulationUI() {
    delete simulationRun;
    CloseWindow();
}

Color SimulationUI::GetCellColor(Cell* cell) {
    CellState state = cell->getCurrentState();
    if (state == INFECTED) return colInfected;
    if (state == IMMUNE) return colImmune;
    if (state == HEALTHY) return colHealthy;
    return BLACK;
}

void SimulationUI::Update() {
    float dt = GetFrameTime();
    float cellSize = (float)gridAreaSize / currentN;

    Vector2 m = GetMousePosition();
    if (m.x < gridAreaSize && m.y < gridAreaSize) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            int r = (int)(m.y / cellSize);
            int c = (int)(m.x / cellSize);
            if (r >= 0 && r < currentN && c >= 0 && c < currentN) {
                simulationRun->spawnInfection(r, c);
            }
        }
    }

    if (!paused) {
        timeAcc += dt;
        if (timeAcc >= updateInterval) {
            simulationRun->updateGame();
            timeAcc = 0;
        }
    }
}

void SimulationUI::DrawGrid(float cellSize) {
    Board& board = simulationRun->getBoard();
    for (int r = 0; r < board.getRows(); r++) {
        for (int c = 0; c < board.getCols(); c++) {
            Cell* cell = board.getCell(r, c);
            DrawRectangleV({(float)c * cellSize, (float)r * cellSize}, {cellSize - 1.0f, cellSize - 1.0f}, GetCellColor(cell));
        }
    }
}

void SimulationUI::DrawSidebar() {
    float sX = (float)gridAreaSize + 15;
    float sW = (float)sidebarWidth - 30;
    float curY = 15;

    DrawRectangleRec({ (float)gridAreaSize, 0, (float)sidebarWidth, (float)screenHeight }, RAYWHITE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 16);

    if (GuiButton({ sX, curY, sW, 40 }, paused ? "#131# START" : "#132# PAUZA")) paused = !paused;
    curY += 50;

    GuiLabel({ sX, curY, sW, 20 }, "Wymiary siatki (max 50):");
    if (GuiValueBox({ sX, curY + 20, 80, 30 }, NULL, &guiN, 5, 50, editModeN)) editModeN = !editModeN;
    if (GuiButton({ sX + 90, curY + 20, sW - 90, 30 }, "ZASTOSUJ")) {
        currentN = guiN;
        delete simulationRun;
        simulationRun = new Run(currentN, currentN);
        simulationRun->setInfectionTime(guiInfTime);
        simulationRun->setImmunityTime(guiImmTime);
        simulationRun->setBoardBounds(bounds);
        paused = true;
    }
    curY += 65;

    GuiLabel({ sX, curY, sW, 20 }, "Ustawienia kolorow:");
    curY += 25;
    float bW = (sW / 3) - 5;
    if (GuiButton({ sX, curY, bW, 30 }, "Chory")) { showPickerInfected = !showPickerInfected; showPickerHealthy = false; showPickerImmune = false; }
    if (GuiButton({ sX + bW + 7, curY, bW, 30 }, "Zdrowy")) { showPickerHealthy = !showPickerHealthy; showPickerInfected = false; showPickerImmune = false; }
    if (GuiButton({ sX + (bW + 7) * 2, curY, bW, 30 }, "Odporny")) { showPickerImmune = !showPickerImmune; showPickerInfected = false; showPickerHealthy = false; }
    curY += 35;

    if (showPickerInfected) GuiColorPicker({ sX, curY, 150, 150 }, "Kolor infekcji", &colInfected);
    if (showPickerHealthy) GuiColorPicker({ sX, curY, 150, 150 }, "Kolor zdrowy", &colHealthy);
    if (showPickerImmune) GuiColorPicker({ sX, curY, 150, 150 }, "Kolor odporny", &colImmune);
    if (showPickerInfected || showPickerHealthy || showPickerImmune) curY += 165; else curY += 10;

    GuiLabel({ sX, curY, sW, 20 }, TextFormat("Szybkosc kroku: %.2fs", updateInterval));
    GuiSliderBar({ sX, curY + 20, sW, 20 }, NULL, NULL, &updateInterval, 0.01f, 1.0f);
    curY += 55;

    GuiLabel({ sX, curY, sW, 20 }, "Czas trwania infekcji:");
    if (GuiSpinner({ sX, curY + 20, sW, 30 }, NULL, &guiInfTime, 1, 100, false)) simulationRun->setInfectionTime(guiInfTime);
    curY += 60;

    GuiLabel({ sX, curY, sW, 20 }, "Czas odpornosci:");
    if (GuiSpinner({ sX, curY + 20, sW, 30 }, NULL, &guiImmTime, 1, 100, false)) simulationRun->setImmunityTime(guiImmTime);
    curY += 60;

    if (GuiCheckBox({ sX, curY, 20, 20 }, "Zamkniete granice", &bounds)) simulationRun->setBoardBounds(bounds);
}

void SimulationUI::Execute() {
    while (!WindowShouldClose()) {
        Update();
        BeginDrawing();
        ClearBackground(colBg);
        DrawGrid((float)gridAreaSize / currentN);
        DrawSidebar();
        DrawText(TextFormat("FPS: %i", GetFPS()), 10, 10, 20, DARKGREEN);
        EndDrawing();
    }
}