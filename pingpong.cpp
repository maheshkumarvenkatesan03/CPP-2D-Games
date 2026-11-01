// Go to last to run

#include "raylib.h"

// Define core entities using structs
struct Ball {
    Vector2 position;
    Vector2 speed;
    float radius;

    void Draw() {
        DrawCircleV(position, radius, WHITE);
    }

    void Update() {
        position.x += speed.x;
        position.y += speed.y;
    }
};

struct Paddle {
    float x, y;
    float width, height;
    float speed;

    Rectangle GetRect() {
        return Rectangle{ x, y, width, height };
    }

    void Draw() {
        DrawRectangleRec(GetRect(), WHITE);
    }
};

int main() {
    // Screen configuration
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib Pong");
    SetTargetFPS(60);

    // Initialize game objects
    Ball ball;
    ball.radius = 10;
    ball.position = Vector2{ (float)screenWidth / 2, (float)screenHeight / 2 };
    ball.speed = Vector2{ 5.0f, 5.0f };

    Paddle leftPaddle;
    leftPaddle.width = 15;
    leftPaddle.height = 100;
    leftPaddle.x = 30;
    leftPaddle.y = (float)screenHeight / 2 - leftPaddle.height / 2;
    leftPaddle.speed = 6.0f;

    Paddle rightPaddle;
    rightPaddle.width = 15;
    rightPaddle.height = 100;
    rightPaddle.x = (float)screenWidth - 30 - rightPaddle.width;
    rightPaddle.y = (float)screenHeight / 2 - rightPaddle.height / 2;
    rightPaddle.speed = 4.5f; // Slightly slower so the player can win

    int leftScore = 0;
    int rightScore = 0;

    // Main game loop
    while (!WindowShouldClose()) {
        // --- 1. PLAYER INPUT (Left Paddle) ---
        if (IsKeyDown(KEY_W) && leftPaddle.y > 0) {
            leftPaddle.y -= leftPaddle.speed;
        }
        if (IsKeyDown(KEY_S) && leftPaddle.y < screenHeight - leftPaddle.height) {
            leftPaddle.y += leftPaddle.speed;
        }

        // --- 2. BASIC AI (Right Paddle) ---
        // Move towards the ball's Y position
        if (ball.position.y < rightPaddle.y + rightPaddle.height / 2 && rightPaddle.y > 0) {
            rightPaddle.y -= rightPaddle.speed;
        }
        if (ball.position.y > rightPaddle.y + rightPaddle.height / 2 && rightPaddle.y < screenHeight - rightPaddle.height) {
            rightPaddle.y += rightPaddle.speed;
        }

        // --- 3. BALL LOGIC & PHYSICS ---
        ball.Update();

        // Top and bottom screen bounds collision
        if (ball.position.y - ball.radius <= 0 || ball.position.y + ball.radius >= screenHeight) {
            ball.speed.y *= -1;
        }

        // Paddle collisions using Raylib's built-in collision check
        if (CheckCollisionCircleRec(ball.position, ball.radius, leftPaddle.GetRect())) {
            if (ball.speed.x < 0) {
                ball.speed.x *= -1.1f; // Slightly increase speed on hit to ramp up difficulty
                ball.speed.y += (ball.position.y - (leftPaddle.y + leftPaddle.height / 2)) / 10.0f; // Add spin
            }
        }
        if (CheckCollisionCircleRec(ball.position, ball.radius, rightPaddle.GetRect())) {
            if (ball.speed.x > 0) {
                ball.speed.x *= -1.1f;
                ball.speed.y += (ball.position.y - (rightPaddle.y + rightPaddle.height / 2)) / 10.0f;
            }
        }

        // Scoring rules
        if (ball.position.x < 0) {
            rightScore++;
            // Reset ball
            ball.position = Vector2{ (float)screenWidth / 2, (float)screenHeight / 2 };
            ball.speed = Vector2{ 5.0f, 5.0f };
        }
        if (ball.position.x > screenWidth) {
            leftScore++;
            // Reset ball
            ball.position = Vector2{ (float)screenWidth / 2, (float)screenHeight / 2 };
            ball.speed = Vector2{ -5.0f, 5.0f };
        }

        // --- 4. RENDER FRAME ---
        BeginDrawing();
        ClearBackground(BLACK);

        // Draw middle divider line
        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, DARKGRAY);

        // Draw entities
        ball.Draw();
        leftPaddle.Draw();
        rightPaddle.Draw();

        // Draw HUD scores
        DrawText(TextFormat("%i", leftScore), screenWidth / 4 - 20, 20, 40, WHITE);
        DrawText(TextFormat("%i", rightScore), (screenWidth / 4) * 3 - 20, 20, 40, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}


// RUN CMD
// -> brew install raylib
// -> clang++ pingpong.cpp -o pingpong -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
// you'll get an executable named "pingpong", finally run the. below
// -> ./pingpong