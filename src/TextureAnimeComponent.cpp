#include "TextureAnimeComponent.h"
#include "GameObject.h"

TextureAnimeComponent::TextureAnimeComponent(AnimeTexture& animeTexture)
	: animeTexture(&animeTexture), frameTime(animeTexture.duration / 1000.0f)
{
}

void TextureAnimeComponent::SetAnimeTexture(AnimeTexture& newAnime) {
	animeTexture = &newAnime;
	frameTime = animeTexture->duration / 1000.0f;
	currentFrame = 0;
	timer = 0.0f;
}

void TextureAnimeComponent::Update() {
	float dt = GetFrameTime();
	timer += dt;

	if (timer >= frameTime) {
		timer = 0.0f;
		currentFrame = (currentFrame + 1) % animeTexture->frames;
	}
}

void TextureAnimeComponent::Draw() {
	TraceLog(LOG_INFO, "TextureAnimeComponent::Draw called");
	if (gameObject == nullptr || animeTexture->texture.id == 0) return;
	float frameWidth = (float)animeTexture->texture.width / animeTexture->frames;
	float frameHeight = (float)animeTexture->texture.height;

	Rectangle sourceRec = {
		currentFrame * frameWidth,
		0.0f,
		frameWidth,
		frameHeight
	};

	Rectangle destRec = {
		gameObject->position.x,
		gameObject->position.y,
		gameObject->scale.x,
		gameObject->scale.y
	};

	Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f };

	DrawTexturePro(animeTexture->texture, sourceRec, destRec, origin, gameObject->rotation, tint);
}