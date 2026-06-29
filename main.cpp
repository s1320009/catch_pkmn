#include "raylib.h"
#include "Ball.h"

int main() {
	// 画面の初期化
	InitWindow(800, 600, "Catch pkmn");
	SetTargetFPS(60);

	Ball ball = CreateBall();

	while (!WindowShouldClose()) {
		// Update
		UpdateBall(&ball);

		// Draw
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawText("Hello, Raylib!", 190, 200, 20, BLUE);
		DrawBall(ball);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}