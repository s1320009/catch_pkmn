#include "Texture.h"

Texture2D bgTexture;

Texture2D pIdle;
Texture2D pThrow;
Texture2D dog;
Texture2D eye;

AnimeTexture pIdleAnime;
AnimeTexture pThrowAnime;
AnimeTexture dogAnime;
AnimeTexture eyeAnime;

void LoadAllTexture() {
	bgTexture = LoadTexture("resources/backColor.png");
	pIdle = LoadTexture("resources/player_idle.png");
	pThrow = LoadTexture("resources/player_throw.png");
	dog = LoadTexture("resources/dog-Sheet.png");
	eye = LoadTexture("resources/eyeEnemy.png");

	pIdleAnime = { pIdle, 7, 100 };
	pThrowAnime = { pThrow, 5, 90 };
	dogAnime = { dog, 8, 100 };
	eyeAnime = { eye, 8, 100 };

	TraceLog(LOG_INFO, "TextureAnimeComponent::Draw called");
}

void UnloadAllTexture() {
	UnloadTexture(bgTexture);
	UnloadTexture(pIdle);
	UnloadTexture(pThrow);
	UnloadTexture(dog);
	UnloadTexture(eye);
}
