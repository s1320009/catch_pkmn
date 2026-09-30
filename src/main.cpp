#include "raylib.h"
#include "Common.h"
#include "Texture.h"
#include "TextureAnimeComponent.h"
#include "Music.h"
#include "Editor.h"
#include "Ball.h"
#include "pkmn.h"
#include "Mewtwo.h"
#include "player.h"
#include "BlinkingText.h"
#include "GameState.h"
#include "StateSelect.h"
#include "Stage.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ContinueSelect.h"
#include "Rule.h"

void ResetGame(GameObject* playerObject, ProjectileManager* projectileManager) {
	playerObject->position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	playerObject->scale = { 50.0f, 50.0f };
	auto* player = playerObject->GetComponent<Player>();
	auto* pAnime = playerObject->GetComponent<TextureAnimeComponent>();
	if (player == nullptr) return;
	player->Reset();
	pAnime->SetAnimeTexture(pIdleAnime);

	// 弾のリセット
	ClearProjectileManager(projectileManager);
	// ポケモンたちの復活
	SceneManager::Instance().ResetAll();
}

void CheckCollisions(Ball* ball, std::vector<PkmnComponent*> pkmnComponents, Player* player) {		//衝突判定はいろんなやつらがぶつかるからここに置く

	player->CheckPlayerHurt(GetMewtwoProjectileManager(), pkmnComponents);

	// ⚽ 2. ボールとポケモンの当たり判定
	if (ball->GetState() == BALL_STATE::FLYING) {
		for (PkmnComponent* pkmnComponent : pkmnComponents) {
			if (not pkmnComponent->IsActive()) continue;
			if (not pkmnComponent->IsVisible()) continue;
			// 円（ボール）と円（ポケモン）の衝突をチェック！
			if (CheckCollisionCircles(ball->GetPosition(), ball->GetRadius(), pkmnComponent->GetPosition(), pkmnComponent->GetRadius())) {

				// 💥 ポケモンに当たったのでボールを跳ね返らせるステートにする！
				ball->SetState(BALL_STATE::BOUNCE);
				pkmnComponent->StartBounce(); // ポケモンも跳ね返るステートにする
				// ① 真上に向かってピョコッと跳ねる初速を与える（上はマイナス）
				ball->SetSpeed({ 0.0f, -6.0f });

				// ② 当たった瞬間のY座標を「天井」の基準として記録しておく！　BOUNCEのほうで初期化するとずっと回るからこっち
				ball->SetBounceStart(ball->GetPosition());
				break;
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
	InitWindow(Common::SCREEN_WIDTH, Common::SCREEN_HEIGHT, "Catch pkmn");
	SetTargetFPS(Common::TARGET_FPS);
	InitAudioDevice();

	//ロード
	Font myFont = LoadFontEx("resources/myFont.ttf", 32, codepoints.data(), codepointCount);
	TraceLog(LOG_INFO, "glyphCount=%d textureId=%u", myFont.glyphCount, myFont.texture.id);

	LoadAllTexture();
	LoadMusic();

	//初期化
	InitializeEditor();
	gameState = STATE_TITLE;			
	
	InitializeStateSelect();
	InitializeContinueSelect();
	BlinkingText text;
	
	GameObject playerObject(0, "Player", "Player");
	auto* player = playerObject.AddComponent<Player>();
	auto* pIdle = playerObject.AddComponent<TextureAnimeComponent>(pIdleAnime);	
	playerObject.position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	playerObject.scale = { 64.0f, 64.0f };

	while (!WindowShouldClose()) {
		UpdateMusic(gameState);
		switch (gameState) {
			case STATE_TITLE:
				UpdateBlinkingText(text);  //処理はこっち

				if (IsKeyPressed(KEY_SPACE)) gameState = STATE_SELECT;
				if (IsKeyPressed(KEY_E)) gameState = STATE_EDITOR;
				break;

			case STATE_SELECT:
				UpdateStateSelect();
				UpdateBlinkingText(text);
				if (STATE_SELECT != gameState) {
					LoadStage(selectRect);
					ResetGame(&playerObject, GetMewtwoProjectileManager());
				}
				break;

			case STATE_RULE:
				UpdateRule(&playerObject, &player->GetBall(),&gameState);
				break;

			case STATE_GAME:
				playerObject.Update();
				gPlayerPosition = playerObject.position;
				SceneManager::Instance().UpdateAll();
				UpdateProjectileManager(GetMewtwoProjectileManager());

				CheckCollisions(&player->GetBall(), SceneManager::Instance().GetPkmnComponents(), player);

				if (not SceneManager::Instance().GetActivePkmnCount()) {
					gameState = STATE_CLEAR;
				}

				if (player->IsDead()) {
					gameState = STATE_CONTINUE;
				}

				if (IsKeyPressed(KEY_P)) {
					gameState = STATE_PAUSE;
				}
				break;

			case STATE_PAUSE:
				// 背景のゲームは動かさない（Updateを一切呼ばないことで「中断」を表現）

				if (IsKeyPressed(KEY_P)) gameState = STATE_GAME;
				if (IsKeyPressed(KEY_R)) gameState = STATE_RULE;
				break;

			case STATE_CONTINUE:
				//背景で敵だけを動かしたいので、プレイヤー以外をUpdate
				
				gPlayerPosition = playerObject.position;
				SceneManager::Instance().UpdateAll();
				UpdateProjectileManager(GetMewtwoProjectileManager());
				UpdateBlinkingText(text);
				UpdateContinueSelect();

				if (IsKeyPressed(KEY_SPACE)) {
					ResetGame(&playerObject, GetMewtwoProjectileManager());
					continueSelectRect = 0; // コンティニュー画面の選択を初期化
				}
				break;

			case STATE_CLEAR:
				UpdateBlinkingText(text);

				if (IsKeyPressed(KEY_SPACE)) {
					ResetGame(&playerObject, GetMewtwoProjectileManager()); 
					gameState = STATE_TITLE;
				}
				break;

			case STATE_EDITOR:
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
			DrawTextEx(myFont, "Catch pkmn", { 500, 300 }, 40, 1, BLACK);
			DrawBlinkingText(text, myFont, "Press SPACE", { 550, 600 }, 20, BLACK);
			break;
		case STATE_SELECT:
			DrawStateSelect();
			DrawTextEx(myFont, "Select stage", { 500, 200 }, 40, 1, BLACK);
			DrawBlinkingText(text, myFont, "Press SPACE", { 550, 600 }, 20, BLACK);
			break;
		case STATE_RULE:
			DrawRule(&playerObject,&player->GetBall());
			DrawBlinkingText(text, myFont, "Press B to back", { 550, 600 }, 20, BLACK);
			break;
		case STATE_GAME:
			DrawTexture(bgTexture, 0, 0, WHITE);
			DrawText("press P to pause", 10, 10, 30, WHITE);
			
			player->GetBall().Draw();
			playerObject.Draw();

			SceneManager::Instance().DrawAll();
			DrawProjectileManager(*GetMewtwoProjectileManager());
			break;
		case STATE_PAUSE:
			DrawTexture(bgTexture, 0, 0, WHITE);

			player->GetBall().Draw();

			//DrawPlayer(player);
			playerObject.Draw();

			SceneManager::Instance().DrawAll();
			DrawProjectileManager(*GetMewtwoProjectileManager());

			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, 150 }); // 半透明の黒いオーバーレイ 色の四つ目の引数がポイント
			DrawTextEx(myFont, "Pause", { 590, 300 }, 40, 1, BLACK);
			break;
		case STATE_CONTINUE:
			DrawTexture(bgTexture, 0, 0, WHITE);
			SceneManager::Instance().DrawAll();
			DrawProjectileManager(*GetMewtwoProjectileManager());

			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 0, 0, 200 }); // 半透明の黒いオーバーレイ poseより濃い
			DrawTextEx(myFont, "Continue ?", { 550, 300 }, 40, 1, WHITE);
			DrawContinueSelect();
			break;
		case STATE_CLEAR:
			// 背景はクリアした瞬間のゲーム画面をそのまま残して
			DrawTexture(bgTexture, 0, 0, WHITE);

			playerObject.Draw();

			player->GetBall().Draw();
			SceneManager::Instance().DrawAll();
			DrawProjectileManager(*GetMewtwoProjectileManager());

			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), { 0, 200, 100, 100 });

			DrawTextEx(myFont, "STAGE CLEAR!", { 440, 300 }, 60, 1, GOLD);
			DrawTextEx(myFont, "THANK YOU FOR PLAYING!", { 460, 450 }, 30, 1, GOLD);
			DrawBlinkingText(text, myFont, "PRESS SPACE", { 550, 600 }, 20, WHITE);
			break;
		case STATE_EDITOR:
			ClearBackground(LIGHTGRAY);
			DrawEditor();
			break;
		}
		
		EndDrawing();
	}

	//アンロード
	UnloadFont(myFont);
	UnloadAllTexture();
	UnloadMusic();
	ShutdownEditor();			//エディタの終了処理

	CloseAudioDevice();
	CloseWindow();
	return 0;
}