#pragma once
#include "raylib.h"
#include "Component.h"
#include <string>
#include <vector>
#include <memory>		// スマートポインタを使うために必要 メモリを安全に管理するためのライブラリ

using namespace std;

// ============================================
// GameObject クラス
// ============================================
class GameObject {
public:
	int id = 0;
	string name;
	string tag;

	//基本パラメータ（Transform）
	Vector2 position = { 0,0 };
	float rotation = 0;
	Vector2 scale = { 1,1 };

	//このオブジェクトが持つComponentのポインタの配列	ポインタじゃないとポリモーフィズムが効かないので、shared_ptrを使う
	vector<shared_ptr<Component>> components; // Componentの配列を持つ

	//コンストラクタ 初期化関数
	GameObject(int uid, const string& objName, const string& objTag = "Untagged"); //(ユニークid, オブジェクト名, タグ（ない時はUntaggedになる）

	template<typename T, typename... Args>	//どんな型でも受け取れるようにするためにテンプレート化する　Argsは可変長テンプレート 引数の数が可変で、型も可変
	T* AddComponent(Args&&... args) { // 可変長テンプレート 引数の数が可変で、型も可変) T*はポインタを返すということ
		auto comp = make_shared<T>(forward<Args>(args)...);		//make_sharedでT型のコンポーネントを作成し、引数をforward(コンストラクタに直送)して渡す
		comp->gameObject = this;		// 新しく作ったコンポーネントにこのGameObjectのポインタを渡す
		components.push_back(comp);	// コンポーネントを配列に追加
		return comp.get();	// 生ポインタを返す
	}

	template<typename T>		//探したいポインタの種類を指定するためにテンプレート化する
	T* GetComponent() {			//見つかったらそのポインタを返す、見つからなかったらnullptrを返す
		for (auto& comp : components) {		//どんな型でもいいから、親クラスComponentのポインタを持つcomponentsのリストを順番に見ていく
			if (auto target = dynamic_cast<T*>(comp.get())) {	// dynamic_castで型をチェックし、T型にキャストできるか確認
				return target;
			}
		}
		return nullptr;	// 見つからなかった場合はnullptrを返す
	}

	void Update();
	void Draw();
};