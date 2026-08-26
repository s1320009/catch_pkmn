#include "Rule.h"
#include "Stage.h"
#include "StateSelect.h"
#include "gameObject.h"
#include "Player.h"
#include <cmath>

static int ruleStep = 0;
static bool initialized = false;
static Vector2 moveStartPos = { 0.0f, 0.0f }; 

//void InitializeRule(Player* player, Ball* ball, PkmnManager* pkmnManager) {
//	*player = CreatePlayer();
//	*ball = CreateBall();
//	LoadStage(selectRect, pkmnManager);
//
//	ruleStep = 0;
//	initialized = true;
//	moveStartPos = player->position;
//}
//
//void UpdateRule(Player* player, Ball* ball, PkmnManager* pkmnManager, GameState* gameState) {
//	if (!initialized) {
//		InitializeRule(player, ball, pkmnManager);
//	}
//
//	switch (ruleStep) {
//		case 0:
//			// このゲームの目的
//			if (IsKeyPressed(KEY_SPACE)) {
//				ruleStep = 1;
//				moveStartPos = player->position;
//			}
//
//			if (IsKeyPressed(KEY_B)) {
//				*gameState = STATE_SELECT;
//			}
//			break;
//		
//		case 1:
//			// 移動の仕方
//			UpdatePlayer(player);
//			
//			{
//				float dx = player->position.x - moveStartPos.x;
//				float dy = player->position.y - moveStartPos.y;
//				if ((dx * dx + dy * dy) > 800.0f) {
//					ruleStep = 2;
//				}
//			}
//
//			if (IsKeyPressed(KEY_B)) {
//				ruleStep = 0;
//				moveStartPos = player->position;
//			}
//
//			break;
//		
//		case 2:
//			// 球の打ち方
//			UpdatePlayer(player);
//			UpdateBall(ball, player);
//		
//			// 1回でも発射したら次へ
//			if (ball->state == BALL_FLYING) {
//				ruleStep = 3;
//			}
//
//			if (IsKeyPressed(KEY_B)) {
//				ruleStep = 1;
//				moveStartPos = player->position;
//			}
//			break;
//
//		case 3:
//			UpdatePlayer(player);
//			UpdateBall(ball, player);
//			UpdatePkmnManager(pkmnManager, player->position);
//
//			// 当たったら次へ
//			if (CheckCollisionCircles(ball->position, ball->radius, pkmnManager->list[0].position, pkmnManager->list[0].blueprint.radius)) {
//				ball->state = BALL_BOUNCE;
//				ball->speed.x = 0.0f;
//				ball->speed.y = -6.0f;
//				ball->bounceStartY = ball->position.y;
//				pkmnManager->list[0].isActive = false;
//				ruleStep = 4;
//			}
//
//			if (IsKeyPressed(KEY_B)) {
//				ruleStep = 2;
//				moveStartPos = player->position;
//			}
//			break;
//		
//		case 4:
//			UpdateBall(ball, player);
//			// まとめ
//			if (IsKeyPressed(KEY_SPACE)) {
//				initialized = false;
//				*gameState = STATE_SELECT;
//				InitializeStateSelect();
//
//			}
//
//			if (IsKeyPressed(KEY_B)) {
//				ruleStep = 3;
//				moveStartPos = player->position;
//				*player = CreatePlayer();
//				*ball = CreateBall();
//				LoadStage(selectRect, pkmnManager);
//			}
//			break;
//	}
//}
//
//void DrawRule(const Player* player,const Ball* ball, const PkmnManager* pkmnManager) {
//	DrawPkmnManager(*pkmnManager);
//
//	if (ruleStep == 0) {
//		DrawText("How to play", 100, 100, 30, BLACK);
//		DrawText("Catch pkmn!", 100, 150, 20, BLACK);
//		DrawText("Don't get hit by pkmn!", 100, 180, 20, BLACK);
//		DrawText("Press SPACE to continue", 100, 220, 20, BLACK);
//	}
//	else if (ruleStep== 1) {
//		DrawText("Drag with left click to move", 100, 100, 20, BLACK);
//		DrawText("Move around to catch pkmn!", 100, 150, 20, BLACK);
//		DrawPlayer(*player);
//		DrawBall(*ball);
//	}
//	else if (ruleStep == 2) {
//		DrawText("Press A/D to charge power", 100, 100, 20, BLACK);
//		DrawText("Then press W/S to charge height", 100, 150, 20, BLACK);
//		DrawText("Finally press SPACE to launch the ball", 100, 200, 20, BLACK);
//		DrawPlayer(*player);
//		DrawBall(*ball);
//	}
//	else if (ruleStep == 3) {
//		DrawText("Hit pkmn with the ball!", 100, 100, 20, BLACK);
//		DrawText("Press B to back to previous step", 100, 150, 20, BLACK);
//		DrawPlayer(*player);
//		DrawBall(*ball);
//	}
//	else if (ruleStep == 4) {
//		DrawText("Good luck!", 100, 100, 30, BLACK);
//		DrawText("Press SPACE to go back to stage select", 100, 150, 20, BLACK);
//		DrawPlayer(*player);
//		DrawBall(*ball);
//	}
//}

//=================================================================================

void InitializeRule(GameObject* playerObject, Ball* ball, PkmnManager* pkmnManager) {
	auto* player = playerObject->GetComponent<Player>();
	if (player == nullptr) return;
	player->Reset();
	
	*ball = CreateBall();
	LoadStage(selectRect, pkmnManager);

	ruleStep = 0;
	initialized = true;
	moveStartPos = playerObject->position;
}

void UpdateRule(GameObject* playerObject, Ball* ball, PkmnManager* pkmnManager, GameState* gameState) {
	auto* player = playerObject->GetComponent<Player>();
	if (!initialized) {
		InitializeRule(playerObject, ball, pkmnManager);
	}

	switch (ruleStep) {
	case 0:
		// このゲームの目的
		if (IsKeyPressed(KEY_SPACE)) {
			ruleStep = 1;
			moveStartPos = playerObject->position;
		}

		if (IsKeyPressed(KEY_B)) {
			*gameState = STATE_SELECT;
		}
		break;

	case 1:
		// 移動の仕方
		playerObject->Update();

		{
			float dx = playerObject->position.x - moveStartPos.x;
			float dy = playerObject->position.y - moveStartPos.y;
			if ((dx * dx + dy * dy) > 800.0f) {
				ruleStep = 2;
			}
		}

		if (IsKeyPressed(KEY_B)) {
			ruleStep = 0;
			moveStartPos = playerObject->position;
		}

		break;

	case 2:
		// 球の打ち方
		playerObject->Update();
		UpdateBall(ball, playerObject);

		// 1回でも発射したら次へ
		if (ball->state == BALL_FLYING) {
			ruleStep = 3;
		}

		if (IsKeyPressed(KEY_B)) {
			ruleStep = 1;
			moveStartPos = playerObject->position;
		}
		break;

	case 3:
		playerObject->Update();
		UpdateBall(ball, playerObject);
		UpdatePkmnManager(pkmnManager, playerObject->position);

		// 当たったら次へ
		if (CheckCollisionCircles(ball->position, ball->radius, pkmnManager->list[0].position, pkmnManager->list[0].blueprint.radius)) {
			ball->state = BALL_BOUNCE;
			ball->speed.x = 0.0f;
			ball->speed.y = -6.0f;
			ball->bounceStartY = ball->position.y;
			pkmnManager->list[0].isActive = false;
			ruleStep = 4;
		}

		if (IsKeyPressed(KEY_B)) {
			ruleStep = 2;
			moveStartPos = playerObject->position;
		}
		break;

	case 4:
		UpdateBall(ball, playerObject);
		// まとめ
		if (IsKeyPressed(KEY_SPACE)) {
			initialized = false;
			*gameState = STATE_SELECT;
			InitializeStateSelect();

		}

		if (IsKeyPressed(KEY_B)) {
			ruleStep = 3;
			moveStartPos = playerObject->position;
			player->Reset();
			*ball = CreateBall();
			LoadStage(selectRect, pkmnManager);
		}
		break;
	}
}

void DrawRule(GameObject* playerObject, const Ball* ball, const PkmnManager* pkmnManager) {
	DrawPkmnManager(*pkmnManager);

	if (ruleStep == 0) {
		DrawText("How to play", 100, 100, 30, BLACK);
		DrawText("Catch pkmn!", 100, 150, 20, BLACK);
		DrawText("Don't get hit by pkmn!", 100, 180, 20, BLACK);
		DrawText("Press SPACE to continue", 100, 220, 20, BLACK);
	}
	else if (ruleStep == 1) {
		DrawText("Drag with left click to move", 100, 100, 20, BLACK);
		DrawText("Move around to catch pkmn!", 100, 150, 20, BLACK);
		DrawBall(*ball);
		playerObject->Draw();
	}
	else if (ruleStep == 2) {
		DrawText("Press A/D to charge power", 100, 100, 20, BLACK);
		DrawText("Then press W/S to charge height", 100, 150, 20, BLACK);
		DrawText("Finally press SPACE to launch the ball", 100, 200, 20, BLACK);
		DrawBall(*ball);
		playerObject->Draw();
	}
	else if (ruleStep == 3) {
		DrawText("Hit pkmn with the ball!", 100, 100, 20, BLACK);
		DrawText("Press B to back to previous step", 100, 150, 20, BLACK);
		DrawBall(*ball);
		playerObject->Draw();
	}
	else if (ruleStep == 4) {
		DrawText("Good luck!", 100, 100, 30, BLACK);
		DrawText("Press SPACE to go back to stage select", 100, 150, 20, BLACK);
		DrawBall(*ball);
		playerObject->Draw();
	}
}