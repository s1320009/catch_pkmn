#include "raylib.h"
#include "Music.h"
#include "Editor.h"
#include "Ball.h"
#include "pkmn.h"
#include "MEWTWO.h"
#include "player.h"
#include "BlinkingText.h"
#include "GameState.h"
#include "StateSelect.h"
#include "Stage.h"
#include "ContinueSelect.h"
#include "Rule.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

void ResetGame(GameObject* playerObject, Ball* ball, PkmnManager* pkmnManager, ProjectileManager* projectileManager) {
//void ResetGame(Player* player, Ball* ball, PkmnManager* pkmnManager, ProjectileManager* projectileManager) {
	// プレイヤーのリセット      GameObject* playerObjectを消して *player = CreatePlayer();にすると旧式・・・・・・・・・1/7
	//player = CreatePlayer(); 
	playerObject->position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	playerObject->scale = { 50.0f, 50.0f };
	auto* player = playerObject->GetComponent<Player>();
	if (player == nullptr) return;
	player->Reset();

	// ボールのリセット
	*ball = CreateBall(); // 丸ごと初期状態で上書き

	// 弾のリセット
	ClearProjectileManager(projectileManager);


	// ポケモンたちの復活
	for (int i = 0; i < pkmnManager->count; i++) {
		pkmnManager->list[i].isActive = true;
		pkmnManager->list[i].state = PKMN_STATE_THINK;
		pkmnManager->list[i].prevState = PKMN_STATE_THINK;
		pkmnManager->list[i].timer = 0.0f;
		pkmnManager->list[i].frameCounter = 0;
		pkmnManager->list[i].position = pkmnManager->list[i].initialPos; // 初期位置に戻す
		// 初期位置に戻したい場合は、各ポケモンの初期位置を構造体に保存しておくのがおすすめです
	}
}

void CheckCollisions(Ball* ball, PkmnManager* pkmnManager, Player* player) {		//衝突判定はいろんなやつらがぶつかるからここに置く　正しいかしらん

	// 🛡️ 1. プレイヤーと敵・弾の当たり判定（CheckPlayerHurt を呼び出す）
	//CheckPlayerHurt(GetMewtwoProjectileManager(), pkmnManager, player);・・・・・・・・・・・2/7
	player->CheckPlayerHurt(GetMewtwoProjectileManager(), pkmnManager);

	// ⚽ 2. ボールとポケモンの当たり判定
	if (ball->state == BALL_FLYING) {
		for (int i = 0; i < pkmnManager->count; i++) {
			Pkmn* enemy = &pkmnManager->list[i];

			if (enemy->isActive && enemy->isVisible) {
				// 円（ボール）と円（ポケモン）の衝突をチェック！
				if (CheckCollisionCircles(ball->position, ball->radius, enemy->position, enemy->blueprint.radius)) {

					// 💥 ポケモンに当たったのでボールを跳ね返らせるステートにする！
					ball->state = BALL_BOUNCE;
					enemy->state = PKMN_STATE_BOUNCE; // ポケモンも跳ね返るステートにする
					// ① 真上に向かってピョコッと跳ねる初速を与える（上はマイナス）
					ball->speed.x = 0.0f;
					ball->speed.y = -6.0f; // ★この数字を大きくすると高く跳ねます

					// ② 当たった瞬間のY座標を「天井」の基準として記録しておく！　BOUNCEのほうで初期化するとずっと回るからこっち
					ball->bounceStartY = ball->position.y;
					break;
				}
			}
		}
	}
}

GameState gameState;

int main() {
	int codepointCount = 0;

	// ASCII + ひらがな + カタカナ + CJK漢字
	int ranges[][2] = {
	{0x0020, 0x007E},
	{0x3040, 0x309F},
	{0x30A0, 0x30FF},
	{0x4E00, 0x9FFF},
	};
	int rangeCount = 4;

	for (int i = 0; i < rangeCount; i++)
		codepointCount += ranges[i][1] - ranges[i][0] + 1;

	vector<int> codepoints(codepointCount);
	int idx = 0;
	for (int i = 0; i < rangeCount; i++) {
		for (int c = ranges[i][0]; c <= ranges[i][1]; c++) {
			codepoints[idx++] = c;
		}
	}

	// 画面の初期化
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Catch pkmn");
	SetTargetFPS(60);
	InitAudioDevice();

	//初期化
	InitializeEditor();
	gameState = STATE_TITLE;			//もちろんタイトルで初期化
	
	InitializeStateSelect();
	InitializeContinueSelect();
	Ball ball = CreateBall();
	PkmnManager pkmnManager{};
	ProjectileManager projectileManager{};
	BlinkingText text;
	
	//Player player = CreatePlayer();・・・・・・・・・・・・・・・・3/7
	GameObject playerObject(0, "Player", "Player");
	auto* player = playerObject.AddComponent<Player>();
	playerObject.position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	playerObject.scale = { 50.0f, 50.0f };

	//ロード
	Texture2D bgTexture = LoadTexture("resources/backColor.png");
	Font japaneseFont = LoadFontEx("resources/KH-Dot-Hibiya-32.ttf", 32, codepoints.data(), codepointCount);
	TraceLog(LOG_INFO, "glyphCount=%d textureId=%u", japaneseFont.glyphCount, japaneseFont.texture.id);

	LoadMusic();

	while (!WindowShouldClose()) {
		UpdateMusic(gameState);
		switch (gameState) {
			case STATE_TITLE:
				// タイトル画面の処理
				UpdateBlinkingText(text);  //処理はこっち

				if (IsKeyPressed(KEY_SPACE)) gameState = STATE_SELECT;
				if (IsKeyPressed(KEY_E)) gameState = STATE_EDITOR;
				break;
			case STATE_SELECT:
				// セレクト画面の処理
				UpdateStateSelect();
				UpdateBlinkingText(text);
				if (STATE_SELECT != gameState) {
					LoadStage(selectRect, &pkmnManager);
					ResetGame(&playerObject, &ball, &pkmnManager, &projectileManager);
					//ResetGame(&player, &ball, &pkmnManager, &projectileManager);・・・・・・・・4/7
				}
				break;
			case STATE_RULE:
				// ルール画面の処理
				UpdateRule(&playerObject, &ball, &pkmnManager, &gameState);
				break;
			case STATE_GAME:
				// ゲーム画面の処理
				// Update

				//UpdatePlayer(&player);・・・・・・・・・・・・・・・・・・・・・・・・・・・・5/7
				//UpdateBall(&ball, &player);
				//UpdatePkmnManager(&pkmnManager, player.position);
				player->Update();
				//UpdateBall(&ball, player);
				UpdateBall(&ball, &playerObject);
				UpdatePkmnManager(&pkmnManager, playerObject.position);
				UpdateProjectileManager(GetMewtwoProjectileManager());

				//CheckCollisions(&ball, &pkmnManager, &player);・・・・・・・・・・・・・・
				CheckCollisions(&ball, &pkmnManager, player);

				// 🌟 プレイヤーが死んだらコンティニュー画面へ！
				if (player->playerState == PLAYER_STATE_DEAD) {
					//if (player.playerState == PLAYER_STATE_DEAD) {・・・・・・・・・・・・・・6/7
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
				
				//UpdatePkmnManager(&pkmnManager, player.position);・・・・・・・・・・・・・・7/7
				UpdatePkmnManager(&pkmnManager, playerObject.position);
				UpdateProjectileManager(GetMewtwoProjectileManager());
				UpdateBlinkingText(text);
				UpdateContinueSelect();

				// 「スペースキーでコンティニュー（今やったステージをリトライ）」
				if (IsKeyPressed(KEY_SPACE)) {
					// 💡 ここでプレイヤーのライフや位置、ポケモンたちをリセットする処理を呼ぶ！
					//ResetGame(&player, &ball, &pkmnManager, GetMewtwoProjectileManager());・・・・・・・・・・・8/7
					ResetGame(&playerObject, &ball, &pkmnManager, GetMewtwoProjectileManager());
					continueSelectRect = 0; // コンティニュー画面の選択を初期化
				}
				break;
			case STATE_CLEAR:
				// クリアの処理
				UpdateBlinkingText(text);

				// 「スペースキーでタイトルに戻る」など
				if (IsKeyPressed(KEY_SPACE)) {
					//ResetGame(&player, &ball, &pkmnManager, GetMewtwoProjectileManager()); ・・・・・・・・・・・・・・9/7
					ResetGame(&playerObject, &ball, &pkmnManager, GetMewtwoProjectileManager()); 
					gameState = STATE_TITLE;
				}
				break;
			case STATE_EDITOR:
				// エディタの処理
				UpdateEditor();
				if (IsKeyPressed(KEY_B)) {
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
		case STATE_SELECT:
			DrawStateSelect();
			DrawTextEx(japaneseFont, "Select stage", { 500, 200 }, 40, 1, BLACK);
			DrawBlinkingText(text, japaneseFont, "Press SPACE", { 550, 600 }, 20, BLACK);
			break;
		case STATE_RULE:
			//DrawRule(&player,&ball,&pkmnManager);・・・・・・・・・・・・・・・・・・・・・・・10/7
			DrawRule(&playerObject,&ball,&pkmnManager);
			DrawBlinkingText(text, japaneseFont, "Press B to back", { 550, 600 }, 20, BLACK);
			break;
		case STATE_GAME:
			DrawTexture(bgTexture, 0, 0, WHITE);
			DrawText("press P to pause", 10, 10, 30, WHITE);

			//DrawPlayer(player);
			player->Draw();
			
			DrawBall(ball);
			DrawPkmnManager(pkmnManager);
			DrawProjectileManager(*GetMewtwoProjectileManager());
			break;
		case STATE_PAUSE:
			DrawTexture(bgTexture, 0, 0, WHITE);

			//DrawPlayer(player);
			player->Draw();

			DrawBall(ball);
			DrawPkmnManager(pkmnManager);
			DrawProjectileManager(*GetMewtwoProjectileManager());

			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, 150 }); // 半透明の黒いオーバーレイ 色の四つ目の引数がポイント
			DrawTextEx(japaneseFont, "Pause", { 590, 300 }, 40, 1, BLACK);
			break;
		case STATE_CONTINUE:
			DrawTexture(bgTexture, 0, 0, WHITE);
			DrawPkmnManager(pkmnManager);
			DrawProjectileManager(*GetMewtwoProjectileManager());

			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, 200 }); // 半透明の黒いオーバーレイ poseより濃い
			DrawTextEx(japaneseFont, "Continue ?", { 550, 300 }, 40, 1, WHITE);
			DrawContinueSelect();
			break;
		case STATE_CLEAR:
			// 背景はクリアした瞬間のゲーム画面をそのまま残して、薄くフィルターをかけるとおしゃれです
			DrawTexture(bgTexture, 0, 0, WHITE);

			//DrawPlayer(player);
			player->Draw();

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
		case STATE_EDITOR:
			ClearBackground(LIGHTGRAY);
			DrawEditor();
			break;
		}
		
		EndDrawing();
	}

	//アンロード
	UnloadFont(japaneseFont);
	UnloadTexture(bgTexture);
	UnloadMusic();
	ShutdownEditor();			//エディタの終了処理

	CloseAudioDevice();
	CloseWindow();
	return 0;
}