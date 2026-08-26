#include "ContinueSelect.h"
#include "GameState.h"

extern GameState gameState;
Rectangle continueRects[3];
int continueSelectRect;

void InitializeContinueSelect() {
	continueSelectRect = 0;
	float selectX = GetScreenWidth() / 2 - (210 + 100);
	for (int i = 0; i < 3; i++) {
		continueRects[i] = { selectX + i * 210, 400, 200, 50 };
	}
}

void UpdateContinueSelect() {
	if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
		if (continueSelectRect == 0) {
			continueSelectRect = 2;
		}
		else
			continueSelectRect -= 1;
	}
	if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
		if (continueSelectRect == 2) {
			continueSelectRect = 0;
		}
		else
			continueSelectRect += 1;
	}
	switch (continueSelectRect) {
	case 0:
		if (IsKeyPressed(KEY_SPACE)) {
			gameState = STATE_GAME;
		}
		break;
	case 1:
		if (IsKeyPressed(KEY_SPACE)) {
			gameState = STATE_RULE;
		}
		break;
	case 2:
		if (IsKeyPressed(KEY_SPACE)) {
			gameState = STATE_TITLE;
		}
		break;
	}
}

void DrawContinueSelect() {
	const char* labels[3] = { "Continue", "Rule", "Title" };
	for (int i = 0; i < 3; i++) {
		if (i == continueSelectRect) {
			DrawRectangleRec(continueRects[i], GREEN);
		}
		else {
			DrawRectangleRec(continueRects[i], WHITE);
		}
		DrawText(labels[i], (int)continueRects[i].x + 50, (int)continueRects[i].y + 20, 20, BLACK);
	}
}