#include "Texture.h"

Texture2D bgTexture;

Texture2D pIdle;
Texture2D pThrow;

AnimeTexture pIdleAnime;
AnimeTexture pThrowAnime;

void LoadAllTexture() {
	bgTexture = LoadTexture("resources/backColor.png");
	pIdle = LoadTexture("resources/player_idle.png");
	pThrow = LoadTexture("resources/player_throw.png");
	pIdleAnime = { pIdle, 7, 100 };
	pThrowAnime = { pThrow, 5, 90 };

	TraceLog(LOG_INFO, "TextureAnimeComponent::Draw called");
}

void UnloadAllTexture() {
	UnloadTexture(bgTexture);
	UnloadTexture(pIdle);
	UnloadTexture(pThrow);
}
