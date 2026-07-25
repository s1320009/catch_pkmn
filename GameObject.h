#pragma once
#include "raylib.h"
#include <string>
#include <vector>
#include <memory>		// スマートポインタを使うために必要 メモリを安全に管理するためのライブラリ

using namespace std;

//前方宣言
class GameObject; // GameObject クラスの前方宣言

// ============================================
// ベースとなる Component クラス
// ============================================
class Component {
public:
	GameObject* gameObject; // アタッチ先のGameObjectへのポインタ &だと参照渡しになるので、nullptrを代入できるようにするためにポインタにする
	// 親クラスに仮想関数を定義することで、親クラスのポインタ(Component*)を通して子クラスの関数を呼び出すことができるようになる つまり子クラスがオーバーライドできるようにするためにvirtual関数が必要
	virtual ~Component() {} // 親クラスに仮想デストラクタを入れることで、子クラスのデストラクタが正しく呼ばれるようにする
	virtual void Update() {} // 親クラスのポインタ(Component*)を通して子クラスのUpdate関数を呼び出すためにvirtual関数にする 子クラスで自由にやってもらうために中身は空っぽ
	virtual void Draw() {} // 親クラスのポインタ(Component*)を通して子クラスのDraw関数を呼び出すためにvirtual関数にする 子クラスで自由にやってもらうために中身は空っぽ
};

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