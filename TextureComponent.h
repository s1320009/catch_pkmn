#pragma once
#include "Component.h"										
#include "raylib.h"											

class TextureComponent : public Component {					// Componentを継承することで、GameObjectにアタッチできるようになる	publicを必ずつける
public:														// publicにすることで、GameObjectからアクセスできるようになる
	Texture2D texture = {};									// テクスチャのデータを保持するための変数
	Color tint = WHITE;										// テクスチャの色を変更するための変数

	explicit TextureComponent(const char* filePath);
	~TextureComponent() override;							//私物（ロード処理など）がある場合、デストラクタをオーバーライドする

	void Draw() override;									//必要な関数のみオーバーライド
};