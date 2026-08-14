#pragma once
#include "Component.h"
#include "raylib.h"

// ============================================
// サンプル: テクスチャ描画コンポーネント
// ============================================
class TextureComponent : public Component {		// Componentを継承することで、GameObjectにアタッチできるようになる	publicを必ずつける
public:											// publicにすることで、GameObjectからアクセスできるようになる
	Texture2D texture = {};						// テクスチャのデータを保持するための変数
	Color tint = WHITE;							// テクスチャの色を変更するための変数

	explicit TextureComponent(const char* filePath);	// コンストラクタでは勝手に暗黙の型変換が起きないようにexplicitをつける　ファイルパスを受け取ってテクスチャをロードするコンストラクタ
	~TextureComponent() override;						// デストラクタをオーバーライドして、テクスチャのメモリを解放する

	void Draw() override;								//必要な関数のみオーバーライド
};