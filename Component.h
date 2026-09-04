#pragma once

//前方宣言
class GameObject; // GameObject クラスの前方宣言

class Component {
public:
	GameObject* gameObject = nullptr;  // アタッチ先のGameObjectへのポインタ &だと参照渡しになるので、nullptrを代入できるようにするためにポインタにする
	// 親クラスに仮想関数を定義することで、親クラスのポインタ(Component*)を通して子クラスの関数を呼び出すことができるようになる つまり子クラスがオーバーライドできるようにするためにvirtual関数が必要
	virtual ~Component() = default; // 親クラスに仮想デストラクタを入れることで、子クラスのデストラクタが正しく呼ばれるようにする	デストラクタにdefaultをつけることで、コンパイラが自動的にデフォルトのデストラクタを生成するようにする
	virtual void Update(); // 親クラスのポインタ(Component*)を通して子クラスのUpdate関数を呼び出すためにvirtual関数にする 子クラスで自由にやってもらうために中身は空っぽ
	virtual void Draw(); // 親クラスのポインタ(Component*)を通して子クラスのDraw関数を呼び出すためにvirtual関数にする 子クラスで自由にやってもらうために中身は空っぽ
};