#include "raylib.h"
#include "Ball.h"
#include "pkmn.h"
#include "MEWTWO.h"
#include "player.h"

#define TODAY_COMMENT ((const char*)u8"karaage")
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

void CheckCollisions(Ball* ball, PkmnManager* pkmnManager, Player* player) {

	// 🛡️ 1. プレイヤーと敵・弾の当たり判定（CheckPlayerHurt を呼び出す）
	CheckPlayerHurt(GetMewtwoProjectileManager(), pkmnManager, player);

	// ⚽ 2. ボールとポケモンの当たり判定
	if (ball->state == BALL_FLYING) {
		for (int i = 0; i < pkmnManager->count; i++) {
			Pkmn* enemy = &pkmnManager->list[i];

			if (enemy->isActive && enemy->isVisible) {
				// 円（ボール）と円（ポケモン）の衝突をチェック！
				if (CheckCollisionCircles(ball->position, ball->radius, enemy->position, enemy->blueprint.radius)) {

					// 💥 ポケモンに当たったのでボールを跳ね返らせるステートにする！
					ball->state = BALL_BOUNCE;

					// ① 真上に向かってピョコッと跳ねる初速を与える（上はマイナス）
					ball->speed.x = 0.0f;
					ball->speed.y = -6.0f; // ★この数字を大きくすると高く跳ねます

					// ② 当たった瞬間のY座標を「天井」の基準として記録しておく！　BOUNCEのほうで初期化するとずっと回るからこっち
					ball->bounceStartY = ball->position.y;
					// ポケモン側を倒す（非アクティブにする）
					enemy->isActive = false;

					break;
				}
			}
		}
	}
}

int main() {
	// 画面の初期化
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Catch pkmn");
	SetTargetFPS(60);

	//初期化
	Player player = CreatePlayer();
	Ball ball = CreateBall();
	PkmnManager pkmnManager = CreatePkmnManager();
	
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

	AddPkmn(&pkmnManager, pika);
	AddPkmn(&pkmnManager, m2);

	//ロード
	Font japaneseFont = LoadFontEx("resources/my_font.ttc", 32, 0, 0);

	while (!WindowShouldClose()) {
		// Update
		UpdatePlayer(&player);
		UpdateBall(&ball , &player);
		UpdatePkmnManager(&pkmnManager);
		UpdateProjectileManager(GetMewtwoProjectileManager());

		CheckCollisions(&ball, &pkmnManager, &player);

		// Draw
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawTextEx(japaneseFont, TODAY_COMMENT, { 190, 200 }, 20, 1, BLUE);
		DrawPlayer(player);
		DrawBall(ball);
		DrawPkmnManager(pkmnManager);
		DrawProjectileManager(*GetMewtwoProjectileManager());
		EndDrawing();
	}

	//アンロード
	UnloadFont(japaneseFont);
	CloseWindow();
	return 0;
}