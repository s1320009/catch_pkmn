#include "GameObject.h"

GameObject::GameObject(int uid, const string& objName, const string& objTag)	//GameObject:: （スコープ解決演算子）ヘッダ側で宣言したやつだと教える役割
	: id(uid), name(objName), tag(objTag) {}	// コンストラクタの初期化リストを使用して、id、name、tagを初期化

void GameObject::Update() {
	for (auto& comp : components) {
		comp->Update();	// コンポーネントのUpdate関数を呼び出す
	}
}

void GameObject::Draw() {
	for (auto& comp : components) {
		comp->Draw();	// コンポーネントのDraw関数を呼び出す
	}
}

