#include "Music.h"

static Music ruleBGM;
static Music gameBGM;
static Music titleBGM;
static Music winBGM;
static Music loseBGM;

static Sound pauseSE;

static bool ruleBGMStarted = false;
static bool  gameBGMStarted = false;
static bool  titleBGMStarted = false;
static bool winBGMStarted = false;
static bool loseBGMStarted = false;
static bool pauseSEStarted = false;

void LoadMusic() {
	ruleBGM = LoadMusicStream("resources/ruleBGM.mp3");
	SetMusicVolume(ruleBGM, 0.01f);
	gameBGM = LoadMusicStream("resources/gameBGM.mp3");
	SetMusicVolume(gameBGM, 0.01f);
	titleBGM = LoadMusicStream("resources/titleBGM.mp3");
	SetMusicVolume(titleBGM, 0.008f);
	winBGM = LoadMusicStream("resources/winBGM.mp3");
	SetMusicVolume(winBGM, 0.01f);
	loseBGM = LoadMusicStream("resources/loseBGM.mp3");
	SetMusicVolume(loseBGM, 0.07f);
	pauseSE = LoadSound("resources/pauseSE.mp3");
}

static void InitMusic() {
	StopMusicStream(ruleBGM);
	StopMusicStream(gameBGM);
	StopMusicStream(titleBGM);
	StopMusicStream(winBGM);
	StopMusicStream(loseBGM);
	StopSound(pauseSE);

	ruleBGMStarted = false;
	gameBGMStarted = false;
	titleBGMStarted = false;
	winBGMStarted = false;
	loseBGMStarted = false;
}

void UpdateMusic(GameState& gameState) {
	static GameState previousState = gameState;

	switch (gameState) {
	case STATE_TITLE :
	case STATE_SELECT:
		if (!titleBGMStarted) {
			InitMusic();
			PlayMusicStream(titleBGM);
			titleBGMStarted = true;
		}
		UpdateMusicStream(titleBGM);
		previousState = gameState;
		break;
	case STATE_RULE:
		if (!ruleBGMStarted) {
			InitMusic();
			PlayMusicStream(ruleBGM);
			ruleBGMStarted = true;
		}
		UpdateMusicStream(ruleBGM);
		previousState = gameState;
		break;
	case STATE_GAME:
		if (!gameBGMStarted) {
			InitMusic();
			PlayMusicStream(gameBGM);
			gameBGMStarted = true;
		}
		StopSound(pauseSE);
		UpdateMusicStream(gameBGM);
		previousState = gameState;
		break;
	case STATE_PAUSE:
		if (previousState != STATE_PAUSE) {
			PlaySound(pauseSE);
		}
		previousState = gameState;
		break;
	case STATE_CLEAR:
		if (!winBGMStarted) {
			InitMusic();
			PlayMusicStream(winBGM);
			winBGMStarted = true;
		}
		UpdateMusicStream(winBGM);
		previousState = gameState;
		break;
	case STATE_CONTINUE:
		if (!loseBGMStarted) {
			InitMusic();
			PlayMusicStream(loseBGM);
			loseBGMStarted = true;
		}
		UpdateMusicStream(loseBGM);
		previousState = gameState;
		break;
	}
}

void UnloadMusic() {
	UnloadMusicStream(ruleBGM);
	UnloadMusicStream(gameBGM);
	UnloadMusicStream(titleBGM);
	UnloadMusicStream(winBGM);
	UnloadMusicStream(loseBGM);
	UnloadSound(pauseSE);
}