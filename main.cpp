#include "raylib.h"
#include "Ball.h"

#define TODAY_COMMENT ((const char*)u8"karaage")

int main() {
	// 画面の初期化
	InitWindow(800, 600, "Catch pkmn");
	SetTargetFPS(60);

	//初期化
	Ball ball = CreateBall();

	//ロード
	Font japaneseFont = LoadFontEx("resources/my_font.ttc", 32, 0, 0);

	while (!WindowShouldClose()) {
		// Update
		UpdateBall(&ball);

		// Draw
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawTextEx(japaneseFont, TODAY_COMMENT, { 190, 200 }, 20, 1, BLUE);
		DrawBall(ball);
		EndDrawing();
	}

	//アンロード
	UnloadFont(japaneseFont);
	CloseWindow();
	return 0;
}