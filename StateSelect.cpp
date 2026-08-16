#include "StateSelect.h"
#include "GameState.h"

#define MAX_RECT 3

extern GameState gameState;

Rectangle selectRects[MAX_RECT];
int selectRect = 1;

void InitializeStateSelect() {
	selectRect = 1;
	float selectY = 300;
	for (int i = 0; i < MAX_RECT; i++) {
		selectRects[i] = { (float)GetScreenWidth() / 2 - 100, selectY + i * 60, 200, 50};
	}
}

void UpdateStateSelect() {
	GameState before = gameState;

	if (IsKeyPressed(KEY_W)	|| IsKeyPressed(KEY_UP)) {
		if (selectRect == 0) {
			selectRect = MAX_RECT-1;
		}
		else 
		selectRect -= 1;	
	}
	if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
		if (selectRect == MAX_RECT-1) {
			selectRect = 0;
		}
		else
		selectRect += 1;
	}
	//selectRectが0のときはルール、1のときはゲーム画面に遷移する
	switch (selectRect) {
		case 0:
			if (IsKeyPressed(KEY_SPACE)) {
				gameState = STATE_RULE;
			}
			break;
		case 1: 
			if (IsKeyPressed(KEY_SPACE)) {
				gameState = STATE_GAME;
			}
			break;
		case 2:
			if (IsKeyPressed(KEY_SPACE)) {
				gameState = STATE_GAME;
			}
			break;
	}

	if (IsKeyPressed(KEY_B)) {
		gameState = STATE_TITLE;
		selectRect = 1;
	}
}

void DrawStateSelect() {
	const char* labels[MAX_RECT] = { "Rule", "Stage1", "Stage2" };
	for (int i = 0; i < MAX_RECT; i++) {
		if (selectRect == i) {
			DrawRectangleRec(selectRects[i], GREEN);
		}
		else {
			DrawRectangleRec(selectRects[i], WHITE);
		}
		DrawText(labels[i], (int)selectRects[i].x + 60, (int)selectRects[i].y + 20, 20, BLACK);
	}
}