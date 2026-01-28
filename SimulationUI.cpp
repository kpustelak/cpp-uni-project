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

    instantTimeChange = true;
    pendingInfTime = guiInfTime;
    pendingImmTime = guiImmTime;

    InitWindow(screenWidth, screenHeight, "LISZAJ SYMULACJA - Kornel Pustelak - 179599");
    SetTargetFPS(60);
}

SimulationUI::~SimulationUI() {
    delete simulationRun;
    CloseWindow();
}
/*Inny sposób implementacji kolorów*/
Color SimulationUI::GetCellColor(Cell* cell) {
    CellState state = cell->getCurrentState();

    if (state == HEALTHY) return colHealthy;

    int row = cell->getX();
    int col = cell->getY();
    auto [isInfected, isImmune, remainingTime, maxTime] = simulationRun->getCellProblemInfo(row, col);

    if (isInfected) {
        if (maxTime > 0) {
            float progress = 1.0f - ((float)remainingTime / (float)maxTime);

            Color result;
            result.r = (unsigned char)(colInfected.r * (1.0f - progress) + colImmune.r * progress);
            result.g = (unsigned char)(colInfected.g * (1.0f - progress) + colImmune.g * progress);
            result.b = (unsigned char)(colInfected.b * (1.0f - progress) + colImmune.b * progress);
            result.a = 255;

            return result;
        }
        return colInfected;
    }

    if (isImmune) {
        if (maxTime > 0) {
            float progress = 1.0f - ((float)remainingTime / (float)maxTime);

            Color result;
            result.r = (unsigned char)(colImmune.r * (1.0f - progress) + colHealthy.r * progress);
            result.g = (unsigned char)(colImmune.g * (1.0f - progress) + colHealthy.g * progress);
            result.b = (unsigned char)(colImmune.b * (1.0f - progress) + colHealthy.b * progress);
            result.a = 255;

            return result;
        }
        return colImmune;
    }

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

    if (GuiButton({ sX, curY, sW, 40 }, "#143# RESET SYMULACJI")) {
        simulationRun->resetBoard();
        paused = true;
    }
    curY += 50;

    GuiLabel({ sX, curY, sW, 20 }, "Wymiary siatki (max 500):");
    if (GuiValueBox({ sX, curY + 20, 80, 30 }, NULL, &guiN, 5, 500, editModeN)) editModeN = !editModeN;

    if (GuiButton({ sX + 90, curY + 20, sW - 90, 30 }, "ZASTOSUJ")) {
        if (guiN != currentN) {
            currentN = guiN;
            simulationRun->resizeBoard(currentN, currentN);
        }
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

    if (GuiCheckBox({ sX, curY, 20, 20 }, "Zmiany czasu natychmiastowe", &instantTimeChange)) {
        if (instantTimeChange) {
            simulationRun->setInfectionTime(guiInfTime);
            simulationRun->setImmunityTime(guiImmTime);
            pendingInfTime = guiInfTime;
            pendingImmTime = guiImmTime;
        }
    }
    curY += 35;

    GuiLabel({ sX, curY, sW, 20 }, "Czas trwania infekcji:");
    int previousInfTime = guiInfTime;
    GuiSpinner({ sX, curY + 20, sW, 30 }, NULL, &guiInfTime, 1, 100, false);
    if (guiInfTime != previousInfTime) {
        pendingInfTime = guiInfTime;
        if (instantTimeChange) {
            simulationRun->setInfectionTime(guiInfTime);
        }
    }
    curY += 60;

    GuiLabel({ sX, curY, sW, 20 }, "Czas odpornosci:");
    int previousImmTime = guiImmTime;
    GuiSpinner({ sX, curY + 20, sW, 30 }, NULL, &guiImmTime, 1, 100, false);
    if (guiImmTime != previousImmTime) {
        pendingImmTime = guiImmTime;
        if (instantTimeChange) {
            simulationRun->setImmunityTime(guiImmTime);
        }
    }
    curY += 60;

    if (!instantTimeChange) {
        bool timesChanged = (pendingInfTime != simulationRun->getInfectionTime() ||
                           pendingImmTime != simulationRun->getImmunityTime());

        GuiSetState(timesChanged ? STATE_NORMAL : STATE_DISABLED);
        if (GuiButton({ sX, curY, sW, 35 }, "ZASTOSUJ CZASY")) {
            if (timesChanged) {
                simulationRun->setInfectionTime(pendingInfTime);
                simulationRun->setImmunityTime(pendingImmTime);
            }
        }
        GuiSetState(STATE_NORMAL);
        curY += 45;
    }

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