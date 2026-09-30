#include "Rule.h"
#include "Stage.h"
#include "StateSelect.h"
#include "gameObject.h"
#include "Player.h"
#include "Scene.h"
#include "SceneManager.h"
#include <cmath>

static int ruleStep = 0;
static bool initialized = false;
static Vector2 moveStartPos = { 0.0f, 0.0f }; 

void InitializeRule(GameObject* playerObject, Ball* ball) {
	auto* player = playerObject->GetComponent<Player>();
	if (player == nullptr) return;
	player->Reset();

	LoadStage(0);

	ruleStep = 0;
	initialized = true;
	moveStartPos = playerObject->position;
}

void UpdateRule(GameObject* playerObject, Ball* ball, GameState* gameState) {
	auto* player = playerObject->GetComponent<Player>();
	if (!initialized) {
		InitializeRule(playerObject, ball);
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
		gPlayerPosition = playerObject->position;
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
		gPlayerPosition = playerObject->position;

		// 1回でも発射したら次へ
		if (ball->GetState() == BALL_STATE::FLYING) {
			ruleStep = 3;
		}

		if (IsKeyPressed(KEY_B)) {
			ruleStep = 1;
			moveStartPos = playerObject->position;
		}
		break;

	case 3:
	{
		playerObject->Update();
		gPlayerPosition = playerObject->position;
		SceneManager::Instance().UpdateAll();

		// 当たったら次へ
		auto pkmnlist = SceneManager::Instance().GetPkmnComponents();
		for (int i = 0; i < pkmnlist.size(); i++) {
			if (CheckCollisionCircles(ball->GetPosition(), ball->GetRadius(), pkmnlist[i]->GetPosition(), pkmnlist[i]->GetRadius())) {
				ball->SetState(BALL_STATE::BOUNCE);
				ball->SetSpeed({ 0.f, -6.f });
				ball->SetBounceStart(ball->GetPosition());
				pkmnlist[i]->SetActive(false);
				ruleStep = 4;
			}
		}

		if (IsKeyPressed(KEY_B)) {
			ruleStep = 2;
			moveStartPos = playerObject->position;
		}
		break;
	}	

	case 4:
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
			LoadStage(selectRect);
		}
		break;
	}
}

void DrawRule(GameObject* playerObject, const Ball* ball) {
	SceneManager::Instance().DrawAll();

	if (ruleStep == 0) {
		DrawText("How to play", 100, 100, 30, BLACK);
		DrawText("Catch pkmn!", 100, 150, 20, BLACK);
		DrawText("Don't get hit by pkmn!", 100, 180, 20, BLACK);
		DrawText("Press SPACE to continue", 100, 220, 20, BLACK);
	}
	else if (ruleStep == 1) {
		DrawText("Drag with left click to move", 100, 100, 20, BLACK);
		DrawText("Move around to catch pkmn!", 100, 150, 20, BLACK);
		ball->Draw();
		playerObject->Draw();
	}
	else if (ruleStep == 2) {
		DrawText("Press A/D to charge power", 100, 100, 20, BLACK);
		DrawText("Then press W/S to charge height", 100, 150, 20, BLACK);
		DrawText("Finally press SPACE to launch the ball", 100, 200, 20, BLACK);
		ball->Draw();
		playerObject->Draw();
	}
	else if (ruleStep == 3) {
		DrawText("Hit pkmn with the ball!", 100, 100, 20, BLACK);
		DrawText("Press B to back to previous step", 100, 150, 20, BLACK);
		ball->Draw();
		playerObject->Draw();
	}
	else if (ruleStep == 4) {
		DrawText("Good luck!", 100, 100, 30, BLACK);
		DrawText("Press SPACE to go back to stage select", 100, 150, 20, BLACK);
		ball->Draw();
		playerObject->Draw();
	}
}