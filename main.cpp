#include "raylib.h"
#include "Ball.h"
#include "pkmn.h"
#include "MEWTWO.h"

#define TODAY_COMMENT ((const char*)u8"karaage")
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720


int main() {
	// 画面の初期化
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Catch pkmn");
	SetTargetFPS(60);

	//初期化
	Ball ball = CreateBall();
	
	PkmnBlueprint pikaSetting = {};				//{0}だとenum型で引っかかってエラーになるので{}で初期化する
	pikaSetting.type = PKMN_PIKACHU;
	pikaSetting.radius = 15.0f;
	pikaSetting.color = YELLOW;
	pikaSetting.dashSpeed = 5.0f;
	pikaSetting.attackduration = 0.5f;
	pikaSetting.thinkduration = 1.0f;
	pikaSetting.stayduration = 1.5f;
	pikaSetting.moveduration = 0.5f;

	PkmnBlueprint m2Setting = {};
	m2Setting.type = PKMN_MEWTWO;
	m2Setting.radius = 20.0f;
	m2Setting.color = PURPLE;
	m2Setting.dashSpeed = 3.0f;
	m2Setting.attackduration = 1.0f;
	m2Setting.thinkduration = 0.5f;
	m2Setting.stayduration = 1.0f;
	m2Setting.moveduration = 0.5f;

	Pkmn pika = CreatePkmn(pikaSetting, { 700, 300 });
	Pkmn m2 = CreatePkmn(m2Setting, { 100, 300 });

	//ロード
	Font japaneseFont = LoadFontEx("resources/my_font.ttc", 32, 0, 0);

	while (!WindowShouldClose()) {
		// Update
		UpdateBall(&ball);
		UpdatePkmn(&pika);
		UpdatePkmn(&m2);
		UpdateProjectileManager(GetMewtwoProjectileManager());

		// Draw
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawTextEx(japaneseFont, TODAY_COMMENT, { 190, 200 }, 20, 1, BLUE);
		DrawBall(ball);
		DrawPkmn(pika);
		DrawPkmn(m2);
		DrawProjectileManager(*GetMewtwoProjectileManager());
		EndDrawing();
	}

	//アンロード
	UnloadFont(japaneseFont);
	CloseWindow();
	return 0;
}