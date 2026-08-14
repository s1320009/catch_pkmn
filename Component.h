#pragma once

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