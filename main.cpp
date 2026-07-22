#include "raylib.h"
#include "Ball.h"
#include "pkmn.h"
#include "MEWTWO.h"
#include "player.h"
#include "BlinkingText.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

typedef enum {
	STATE_TITLE,
	STATE_RULE,
	STATE_GAME,
	STATE_PAUSE,
	STATE_CONTINUE,
	STATE_CLEAR
} GameState;

void ResetGame(Player* player, Ball* ball, PkmnManager* pkmnManager) {
	// プレイヤーのリセット
	player->life = 3;
	player->position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	player->speed = { 0.0f, 0.0f };
	player->state = PLAYER_STATE_FINE;
	player->invincibleFrame = 0;
	player->isInvincible = false;

	// ボールのリセット
	*ball = CreateBall(); // 丸ごと初期状態で上書き

	// ポケモンたちの復活
	for (int i = 0; i < pkmnManager->count; i++) {
		pkmnManager->list[i].isActive = true;
		pkmnManager->list[i].state = PKMN_STATE_THINK;
		pkmnManager->list[i].timer = 0.0f;
		// 初期位置に戻したい場合は、各ポケモンの初期位置を構造体に保存しておくのがおすすめです
	}
}

void CheckCollisions(Ball* ball, PkmnManager* pkmnManager, Player* player) {		//衝突判定はいろんなやつらがぶつかるからここに置く　正しいかしらん

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
	GameState gameState = STATE_TITLE;			//もちろんタイトルで初期化
	Player player = CreatePlayer();
	Ball ball = CreateBall();
	PkmnManager pkmnManager = CreatePkmnManager();
	BlinkingText text;
	
	PkmnBlueprint pikaSetting = {};				//{0}だとenum型で引っかかってエラーになるので{}で初期化する
	pikaSetting.type = PKMN_PIKACHU;
	pikaSetting.radius = 15.0f;
	pikaSetting.color = YELLOW;
	pikaSetting.dashSpeed = 9.0f;
	pikaSetting.attackduration = 0.5f;
	pikaSetting.thinkduration = 1.0f;
	pikaSetting.stayduration = 1.5f;
	pikaSetting.moveduration = 0.5f;

	PkmnBlueprint m2Setting = {};
	m2Setting.type = PKMN_MEWTWO;
	m2Setting.radius = 20.0f;
	m2Setting.color = PURPLE;
	m2Setting.dashSpeed = 7.0f;
	m2Setting.attackduration = 1.0f;
	m2Setting.thinkduration = 0.5f;
	m2Setting.stayduration = 1.0f;
	m2Setting.moveduration = 0.5f;

	Pkmn pika = CreatePkmn(pikaSetting, { 700, 300 });
	Pkmn m2 = CreatePkmn(m2Setting, { 100, 300 });

	AddPkmn(&pkmnManager, pika);
	AddPkmn(&pkmnManager, m2);

	//ロード
	Texture2D bgTexture = LoadTexture("resources/backColor.png");
	Font japaneseFont = LoadFontEx("resources/my_font.ttc", 32, 0, 0);

	while (!WindowShouldClose()) {
		switch (gameState) {
			case STATE_TITLE:
				// タイトル画面の処理
				UpdateBlinkingText(text);  //処理はこっち

				if (IsKeyPressed(KEY_SPACE)) gameState = STATE_GAME;
				if (IsKeyPressed(KEY_R)) gameState = STATE_RULE;
				break;
			case STATE_RULE:
				// ルール画面の処理
				if (IsKeyPressed(KEY_B)) gameState = STATE_TITLE;
				break;
			case STATE_GAME:
				// ゲーム画面の処理
				// Update
				UpdatePlayer(&player);
				UpdateBall(&ball, &player);
				UpdatePkmnManager(&pkmnManager, player.position);
				UpdateProjectileManager(GetMewtwoProjectileManager());

				CheckCollisions(&ball, &pkmnManager, &player);

				// 🌟 プレイヤーが死んだらコンティニュー画面へ！
				if (player.state == PLAYER_STATE_DEAD) {
					gameState = STATE_CONTINUE;
				}

				// 🌟 アクティブなポケモンがいなくなったらクリア画面へ！
				if (!IsAnyPkmnActive(pkmnManager)) {
					gameState = STATE_CLEAR;
				}

				// 🌟 Pキーが押されたらポーズ画面へ！
				if (IsKeyPressed(KEY_P)) {
					gameState = STATE_PAUSE;
				}
				break;
			case STATE_PAUSE:
				// 一時停止の処理
				// 背景のゲームは動かさない（Updateを一切呼ばないことで「中断」を表現！）

				// 「Pキーで再開（STATE_GAMEへ）」
				if (IsKeyPressed(KEY_P)) gameState = STATE_GAME;
				// 「Rキーでルール説明へ」
				if (IsKeyPressed(KEY_R)) gameState = STATE_RULE;
				break;
			case STATE_CONTINUE:
				// 続行の処理
				// 🌟 背景で敵だけを動かしたいので、プレイヤー以外をUpdateする！
				UpdatePkmnManager(&pkmnManager, player.position);
				UpdateProjectileManager(GetMewtwoProjectileManager());
				UpdateBlinkingText(text);

				// 「スペースキーでコンティニュー（今やったステージをリトライ）」
				if (IsKeyPressed(KEY_SPACE)) {
					// 💡 ここでプレイヤーのライフや位置、ポケモンたちをリセットする処理を呼ぶ！
					ResetGame(&player, &ball, &pkmnManager);
					gameState = STATE_GAME;
				}
				// 「Tキーでタイトルに戻る」
				if (IsKeyPressed(KEY_T)) {
					ResetGame(&player, &ball, &pkmnManager);
					gameState = STATE_TITLE;
				}
				break;
			case STATE_CLEAR:
				// クリアの処理
				UpdateBlinkingText(text);

				// 「スペースキーでタイトルに戻る」など
				if (IsKeyPressed(KEY_SPACE)) {
					ResetGame(&player, &ball, &pkmnManager); // ゲームをリセット
					gameState = STATE_TITLE;
				}
				break;
		}		

		// Draw
		BeginDrawing();
		ClearBackground(RAYWHITE);

		switch (gameState) {
		case STATE_TITLE:
			DrawTextEx(japaneseFont, "Catch pkmn", { 500, 300 }, 40, 1, BLACK);
			DrawBlinkingText(text, japaneseFont, "Press SPACE", { 550, 600 }, 20, BLACK);
			break;
		case STATE_RULE:
			DrawTexture(bgTexture, 0, 0, WHITE);
			DrawTextEx(japaneseFont, "Rules description", { 500, 300 }, 40, 1, BLACK);
			break;
		case STATE_GAME:
			DrawTexture(bgTexture, 0, 0, WHITE);
			DrawPlayer(player);
			DrawBall(ball);
			DrawPkmnManager(pkmnManager);
			DrawProjectileManager(*GetMewtwoProjectileManager());
			break;
		case STATE_PAUSE:
			DrawTexture(bgTexture, 0, 0, WHITE);
			DrawPlayer(player);
			DrawBall(ball);
			DrawPkmnManager(pkmnManager);
			DrawProjectileManager(*GetMewtwoProjectileManager());

			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, 150 }); // 半透明の黒いオーバーレイ 色の四つ目の引数がポイント
			DrawTextEx(japaneseFont, "Pause", { 500, 300 }, 40, 1, BLACK);
			break;
		case STATE_CONTINUE:
			DrawTexture(bgTexture, 0, 0, WHITE);
			DrawPkmnManager(pkmnManager);
			DrawProjectileManager(*GetMewtwoProjectileManager());

			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, 200 }); // 半透明の黒いオーバーレイ poseより濃い
			DrawTextEx(japaneseFont, "Continue ?", { 550, 300 }, 40, 1, BLACK);
			DrawBlinkingText(text, japaneseFont, "Press SPACE", { 550, 600 }, 20, BLACK);
			break;
		case STATE_CLEAR:
			// 背景はクリアした瞬間のゲーム画面をそのまま残して、薄くフィルターをかけるとおしゃれです
			DrawTexture(bgTexture, 0, 0, WHITE);
			DrawPlayer(player);
			DrawBall(ball);
			DrawPkmnManager(pkmnManager);
			DrawProjectileManager(*GetMewtwoProjectileManager());

			// 緑がかった半透明のフィルターで「さわやかなクリア感」を出す
			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 200, 100, 100 });

			// クリアの文字と操作案内
			DrawTextEx(japaneseFont, "STAGE CLEAR!", { 440, 300 }, 60, 1, GOLD);
			DrawTextEx(japaneseFont, "THANK YOU FOR PLAYING!", { 460, 450 }, 30, 1, GOLD);
			DrawBlinkingText(text, japaneseFont, "PRESS SPACE", { 550, 600 }, 20, WHITE);
			break;
		}
		
		EndDrawing();
	}

	//アンロード
	UnloadFont(japaneseFont);
	UnloadTexture(bgTexture);
	CloseWindow();
	return 0;
}