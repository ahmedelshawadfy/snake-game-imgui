#include "SnakeGame.h"
#include <cstdlib>
#include <ctime>
#include <imgui.h>

SnakeGame::SnakeGame(int gridWidth, int gridHeight)
    : gridWidth(gridWidth), gridHeight(gridHeight), score(0),
    gameOver(false), paused(false), waitingForStart(true), moveTimer(0.0f), moveDelay(0.1f),
    currentDir(Direction::RIGHT), nextDir(Direction::RIGHT)
{
    srand(static_cast<unsigned>(time(0)));

    // Initialize snake with 3 segments
    snake.push_back({ gridWidth / 2, gridHeight / 2 });
    snake.push_back({ gridWidth / 2 - 1, gridHeight / 2 });
    snake.push_back({ gridWidth / 2 - 2, gridHeight / 2 });

    SpawnFood();
}

void SnakeGame::Update(float deltaTime)
{
    if (gameOver || paused || waitingForStart) return;

    moveTimer += deltaTime;
    if (moveTimer >= moveDelay)
    {
        moveTimer = 0.0f;
        currentDir = nextDir;
        MoveSnake();
        CheckCollisions();
    }
}

void SnakeGame::Render(ImDrawList* drawList, ImVec2 canvasPos, float cellSize)
{
    ImU32 snakeColor = ImGui::GetColorU32(ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
    ImU32 foodColor = ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
    ImU32 gridColor = ImGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.3f, 1.0f));

    // Draw grid
    for (int i = 0; i <= gridWidth; ++i)
    {
        ImVec2 start(canvasPos.x + i * cellSize, canvasPos.y);
        ImVec2 end(canvasPos.x + i * cellSize, canvasPos.y + gridHeight * cellSize);
        drawList->AddLine(start, end, gridColor, 1.0f);
    }
    for (int i = 0; i <= gridHeight; ++i)
    {
        ImVec2 start(canvasPos.x, canvasPos.y + i * cellSize);
        ImVec2 end(canvasPos.x + gridWidth * cellSize, canvasPos.y + i * cellSize);
        drawList->AddLine(start, end, gridColor, 1.0f);
    }

    // Draw snake
    for (const auto& segment : snake)
    {
        ImVec2 min(canvasPos.x + segment.x * cellSize, canvasPos.y + segment.y * cellSize);
        ImVec2 max(min.x + cellSize, min.y + cellSize);
        drawList->AddRectFilled(min, max, snakeColor);
    }

    // Draw food
    ImVec2 foodMin(canvasPos.x + food.x * cellSize, canvasPos.y + food.y * cellSize);
    ImVec2 foodMax(foodMin.x + cellSize, foodMin.y + cellSize);
    drawList->AddRectFilled(foodMin, foodMax, foodColor);
}

void SnakeGame::SetDirection(Direction dir)
{
    // If waiting for start, start the game on first arrow input
    if (waitingForStart)
    {
        waitingForStart = false;
        currentDir = dir;
        nextDir = dir;
        return;
    }

    // Prevent snake from reversing into itself
    if ((currentDir == Direction::UP && dir == Direction::DOWN) ||
        (currentDir == Direction::DOWN && dir == Direction::UP) ||
        (currentDir == Direction::LEFT && dir == Direction::RIGHT) ||
        (currentDir == Direction::RIGHT && dir == Direction::LEFT))
        return;

    nextDir = dir;
}

void SnakeGame::Reset()
{
    snake.clear();
    snake.push_back({ gridWidth / 2, gridHeight / 2 });
    snake.push_back({ gridWidth / 2 - 1, gridHeight / 2 });
    snake.push_back({ gridWidth / 2 - 2, gridHeight / 2 });

    score = 0;
    gameOver = false;
    paused = false;
    waitingForStart = true;  // Reset waiting for start flag
    moveTimer = 0.0f;
    currentDir = Direction::RIGHT;
    nextDir = Direction::RIGHT;

    SpawnFood();
}

void SnakeGame::StartGame()
{
    waitingForStart = false;
}

void SnakeGame::SpawnFood()
{
    bool validPosition = false;
    while (!validPosition)
    {
        food.x = rand() % gridWidth;
        food.y = rand() % gridHeight;

        validPosition = true;
        for (const auto& segment : snake)
        {
            if (segment.x == food.x && segment.y == food.y)
            {
                validPosition = false;
                break;
            }
        }
    }
}

void SnakeGame::MoveSnake()
{
    Segment newHead = snake[0];

    switch (currentDir)
    {
    case Direction::UP:    newHead.y--; break;
    case Direction::DOWN:  newHead.y++; break;
    case Direction::LEFT:  newHead.x--; break;
    case Direction::RIGHT: newHead.x++; break;
    }

    snake.insert(snake.begin(), newHead);

    // Check if food is eaten
    if (newHead.x == food.x && newHead.y == food.y)
    {
        score += 10;
        SpawnFood();
    }
    else
    {
        snake.pop_back();
    }
}

void SnakeGame::CheckCollisions()
{
    Segment head = snake[0];

    // Wall collision - game over when hitting boundaries
    if (head.x < 0 || head.x >= gridWidth || head.y < 0 || head.y >= gridHeight)
    {
        gameOver = true;
        return;
    }

    // Self collision - game over when hitting own body
    for (size_t i = 1; i < snake.size(); ++i)
    {
        if (head.x == snake[i].x && head.y == snake[i].y)
        {
            gameOver = true;
            return;
        }
    }
}
