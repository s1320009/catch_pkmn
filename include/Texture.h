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

extern AnimeTexture pIdleAnime;
extern AnimeTexture pThrowAnime;

void LoadAllTexture();
void UnloadAllTexture();