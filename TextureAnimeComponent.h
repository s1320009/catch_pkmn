#pragma once
#include "Component.h"
#include "raylib.h"

class TextureAnimeComponent : public Component {
public:
	Texture2D texture = {};
	Color tint = WHITE;

	int totalFrames = 1;
	int currentFrame = 0;
	float frameTime = 0.1f; // 1フレームの表示時間（秒）
	float timer = 0.0f; // フレーム切り替え用のタイマー
	float frameWidth = 0.0f; // 1フレームの幅
	float frameHeight = 0.0f; // 1フレームの高さ

	TextureAnimeComponent(const char* filePath, int frames, int duration);
	~TextureAnimeComponent() override;
	void Update() override;
	void Draw() override;
};