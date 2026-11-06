#pragma once
#include "raylib.h"

struct AnimeTexture {
	Texture2D texture;
	int frames;
	float duration;
};


extern Texture2D bgTexture;
extern Texture2D pIdle;
extern Texture2D pThrow;
extern Texture2D dog;
extern Texture2D eye;

extern AnimeTexture pIdleAnime;
extern AnimeTexture pThrowAnime;
extern AnimeTexture dogAnime;
extern AnimeTexture eyeAnime;

void LoadAllTexture();
void UnloadAllTexture();