#include "raylib.h"

int main() {
	// 画面の初期化
	InitWindow(800, 600, "Catch pkmn");
	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		// Update

		// Draw
		BeginDrawing();
		ClearBackground(RAYWHITE);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}