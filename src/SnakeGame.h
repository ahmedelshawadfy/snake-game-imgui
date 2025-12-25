#pragma once
#include <vector>
#include <queue>
#include <imgui.h>

enum class Direction { UP, DOWN, LEFT, RIGHT };

struct Segment {
    int x, y;
};

class SnakeGame
{
public:
    SnakeGame(int gridWidth = 20, int gridHeight = 20);
    ~SnakeGame() = default;

    void Update(float deltaTime);
    void Render(ImDrawList* drawList, ImVec2 canvasPos, float cellSize);
    void SetDirection(Direction dir);
    void Reset();
    void StartGame();

    bool IsGameOver() const { return gameOver; }
    bool IsGamePaused() const { return paused; }
    void SetPaused(bool state) { paused = state; }
    int GetScore() const { return score; }
    bool IsWaitingForStart() const { return waitingForStart; }

private:
    void SpawnFood();
    void MoveSnake();
    void CheckCollisions();

    std::vector<Segment> snake;
    Segment food;
    Direction currentDir, nextDir;
    int gridWidth, gridHeight;
    int score;
    bool gameOver, paused;
    bool waitingForStart;  // Flag to wait for first arrow input
    float moveTimer, moveDelay;
};
