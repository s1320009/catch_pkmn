#include "TextureComponent.h"
#include "GameObject.h"

// ============================================
// TextureComponent
// ============================================
TextureComponent::TextureComponent(const char* filePath) {
    texture = LoadTexture(filePath);	// テクスチャをロード	textureはAddComponent（make_shared）でGameObjectにアタッチされるときに実体化するのでそれぞれ別の場所を持っている
}

TextureComponent::~TextureComponent() {
    if (texture.id != 0) {		// テクスチャがidを確認してロードされている場合のみ解放
        UnloadTexture(texture);	// テクスチャのメモリを解放
    }
}

void TextureComponent::Draw() {
    if (gameObject == nullptr || texture.id == 0) return;       // GameObjectがnullptrの場合や、テクスチャがロードされていない場合は描画しない

    Rectangle sourceRec = {                                     // テクスチャの描画範囲を指定するためのRectangle構造体を作成　（全部）
        0.0f,
        0.0f,
        (float)texture.width,
        (float)texture.height
    };

    Rectangle destRec = {										// 描画先の位置とサイズを指定するためのRectangle構造体を作成 (GameObjectのスケールの変更に合わせてテクスチャの大きさも変わる)
        gameObject->position.x,
        gameObject->position.y,
        texture.width * gameObject->scale.x,
        texture.height * gameObject->scale.y
    };

    Vector2 origin = { destRec.width / 2.0f, destRec.height / 2.0f }; // 描画の中心を設定
    //下の関数が描画の中心を原点として回転させるので、原点をテクスチャの中心に設定する
    DrawTexturePro(texture, sourceRec, destRec, origin, gameObject->rotation, tint);	//(テクスチャ、描画元の矩形、描画先の矩形、原点、回転角度、色)を指定して描画
}
