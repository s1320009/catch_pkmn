#pragma once
#include "Component.h"
#include "Texture.h"
#include "raylib.h"

class TextureAnimeComponent : public Component {
public:
	AnimeTexture* animeTexture = nullptr;
	Color tint = WHITE;

	int currentFrame = 0;
	float frameTime = 0.1f; // 1フレームの表示時間（秒）
	float timer = 0.0f; // フレーム切り替え用のタイマー

	TextureAnimeComponent(AnimeTexture& animeTexture);
	void SetAnimeTexture(AnimeTexture& newAnime);
	void Update() override;
	void Draw() override;
};