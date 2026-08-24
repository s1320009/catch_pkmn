#include "TextureAnimeComponent.h"
#include "GameObject.h"

TextureAnimeComponent::TextureAnimeComponent(const char* filePath, int frames, int duration) 
	: totalFrames(frames > 0 ? frames : 1), frameTime(duration)
{
	texture = LoadTexture(filePath);
	frameWidth = (float)texture.width / totalFrames;
	frameHeight = (float)texture.height / totalFrames;
}

TextureAnimeComponent::~TextureAnimeComponent() {
	if (texture.id != 0) {
		UnloadTexture(texture);
	}
}

void TextureAnimeComponent::Update() {
	float dt = GetFrameTime();
	timer += dt;

	if (timer > frameTime) {
		timer = 0;
		currentFrame = (currentFrame + 1) % totalFrames;
	}
}

void TextureAnimeComponent::Draw() {
	if (gameObject == nullptr || texture.id == 0) return;

	Rectangle sourceRec = {
		currentFrame * frameWidth,
		0.0f,
		frameWidth,
		frameHeight
	};

	Rectangle destRec = {
		gameObject->position.x,
		gameObject->position.y,
		frameWidth * gameObject->scale.x,
		frameHeight * gameObject->scale.y
	};

	Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };

	DrawTexturePro(texture, sourceRec, destRec, origin, gameObject->rotation, tint);
}