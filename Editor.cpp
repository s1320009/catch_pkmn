#include "Editor.h"
#include "raylib.h"
#include <cstdio>		//stdio.hを使うために必要
#include <string>

namespace //namespace {}で無名空間を作る　staticの領域展開バージョン　これはこのcppファイル内でしか使えない変数や関数を定義するためのもの　ヘッダには書かない
{
	constexpr int HIERARCHY_WIDTH = 220;
	constexpr int INSPECTOR_WIDTH = 300;
	constexpr int SCENE_TOP = 80;

	vector<shared_ptr<GameObject>> editorObjects;	//Editorで作ったGameObjectの配列	スマートポインタでオブジェクトを楽々管理
	GameObject* selectedObject = nullptr;	//選択されたGameObjectのポインタ	生ポインタは軽くて便利　選ぶだけで所有権は持たないのでオブジェクトの寿命管理は上のやつに任せる

	int nextObjectId = 1;	//次に作るGameObjectのIDを管理する変数
	bool isDragging = false;	//ドラッグ中かどうかを管理する変数
	Vector2 dragOffset = { 0,0 };		//オフセット（オブジェクトの位置ークリックした位置）　新しい位置はマウスの位置＋オフセット

	//シーンビューの矩形を取得する関数
	Rectangle GetSceneRect() {
		return {													//static_cast<float>()でint型をfloat型に変換する	ただの(float)だと見にくいのでstatic_castを使う
			static_cast<float>(HIERARCHY_WIDTH),	//左端の位置
			static_cast<float>(SCENE_TOP),	//上端の位置
			static_cast<float>(GetScreenWidth() - HIERARCHY_WIDTH - INSPECTOR_WIDTH),	//幅
			static_cast<float>(GetScreenHeight() - SCENE_TOP)	//高さ
		};
	}

	//インスペクターの矩形を取得する関数
	Rectangle GetInspectorRect() {
		return {
			static_cast<float>(GetScreenWidth() - INSPECTOR_WIDTH),	//左端の位置
			0.0f, //上端の位置
			static_cast<float>(INSPECTOR_WIDTH),	//幅
			static_cast<float>(GetScreenHeight())	//高さ
		};
	}

	//マウスの位置がシーンビューの中にあるかどうかを判定する関数
	bool IsPointInScene(Vector2 point)
	{
		return CheckCollisionPointRec(point, GetSceneRect());
	}

	//新しい空のGameObjectを追加する関数
	void AddEmptyObject() {								//vector<shared_ptr<GameObject>> editorObjects;のリストに新しいGameObjectを追加する関数
		auto object = make_shared<GameObject>(			//make_shared　shared_ptr<GameObject>というスマートポインタの型を作る関数　（）の中身をGameObjectのコンストラクタに投げてできたやつのポインタを返す
			nextObjectId++,	//ユニークIDを付与
			"GameObject" + to_string(nextObjectId - 1),	//名前を付与
			"Untagged"	//タグを付与
		);

		Rectangle scene = GetSceneRect();	//シーンビューの矩形を取得

		object->position = {
			scene.x + scene.width / 2.0f,	//シーンビューの中心に配置
			scene.y + scene.height / 2.0f
		};

		object->rotation = 0.0f;	//回転を0に設定
		object->scale = { 1,1 };	//スケールを1に設定

		editorObjects.push_back(object);	//editorObjectsのリストに追加
		selectedObject = object.get();	//選択されたオブジェクトを新しく作ったオブジェクトに設定  shared_ptrのget()で生ポインタを取得する
	}

	//マウスの位置にあるオブジェクトを探す関数
	GameObject* FindObjectAtPosition(Vector2 point) {
		//手前に見える(後から追加した)オブジェクトを優先して選択する
		for (auto it = editorObjects.rbegin(); it != editorObjects.rend(); ++it) {
			GameObject* object = it->get();	//shared_ptrから生ポインタを取得

			Rectangle objectRect = {		//マウスが重なってるか見るために必要
				object->position.x - 35,	//オブジェクトの中心を基準に矩形を作るために半分の幅を引く
				object->position.y - 25,	//オブジェクトの中心を基準に矩形を作るために半分の高さを引く
				70 * object->scale.x,
				50 * object->scale.y
			};

			if (CheckCollisionPointRec(point, objectRect)) {	//マウスの位置がオブジェクトの矩形の中にあるかどうかを判定
				return object;	//見つかったらそのオブジェクトのポインタを返す
			}
		}
		return nullptr;	//見つからなかった場合はnullptrを返す
	}

	//オブジェクトをシーンビューに描画する関数
	void DrawObjectInScene(GameObject* object) {		//GameObject* で&じゃないのは、shared_ptrのget()で生ポインタを取得して渡すから
		Rectangle objectRect = {
			object->position.x - 35,
			object->position.y - 25,
			70 * object->scale.x,
			50 * object->scale.y
		};
		//選択されているオブジェクトは色を変える
		Color objectColor = (object == selectedObject) ? GOLD : SKYBLUE;

		DrawRectangleRec(objectRect, objectColor);	//オブジェクトの矩形を描画
		DrawRectangleLinesEx(objectRect, 2.0f, BLACK);	//オブジェクトの矩形の枠線を描画

		DrawText(
			object->name.c_str(),		//char*型に変換するためにc_str()を使う
			static_cast<int>(objectRect.x),
			static_cast<int>(objectRect.y + objectRect.height + 5.0f),
			14,
			BLACK
		);

		if (object == selectedObject) {
			DrawCircleV(object->position, 5.0f, RED);	//選択されているオブジェクトの中心に赤い円を描画
		}
	}

	//ヒエラルキーを描画する関数
	void DrawHierarchy() {
		DrawRectangle(
			0,
			0,
			HIERARCHY_WIDTH,
			GetScreenHeight(),
			LIGHTGRAY
		);

		DrawText("Hierarchy", 10, 10, 20, BLACK);

		Rectangle addButton = { 10, 40, 190, 30 };

		DrawRectangleRec(addButton, DARKGREEN);
		DrawText("Add", 20, 45, 20, BLACK);

		int objectY = 100;

		for (const auto& object : editorObjects) {
			Rectangle itemRect = {
				10,
				static_cast<float>(objectY),
				200,
				30
			};

			if (object.get() == selectedObject) {
				DrawRectangleRec(itemRect, SKYBLUE);
			}

			objectY += 35;
		}

		DrawText("Click to select an object", 10, GetScreenHeight() - 30, 14, DARKGRAY);
	}

	void DrawScene() {
		Rectangle scene = GetSceneRect();

		DrawRectangleRec(scene, LIGHTGRAY);

		//Scene上部のタイトルバー
		DrawRectangle(
			static_cast<int>(scene.x),
			0,
			static_cast<int>(scene.width),
			SCENE_TOP,
			Color{ 70,70,80,255 }
		);

		DrawText("SCENE", static_cast<int>(scene.x) + 10, 10, 20, WHITE);
		DrawText(
			"Drag objects with the mouse",
			static_cast<int>(scene.x) + 10,
			40,
			14,
			LIGHTGRAY
		);

		//Sceneのグリッド
		for (int x = static_cast<int>(scene.x); x < scene.x + scene.width; x += 40)
		{
			DrawLine(x, SCENE_TOP, x, GetScreenHeight(), LIGHTGRAY);
		}

		for (int y = SCENE_TOP; y < GetScreenHeight(); y += 40)
		{
			DrawLine(static_cast<int>(scene.x), y, static_cast<int>(scene.x + scene.width), y, LIGHTGRAY);
		}

		for (const auto& object : editorObjects)
		{
			DrawObjectInScene(object.get());
		}
	}

	void DrawInspector() {
		Rectangle inspector = GetInspectorRect();

		DrawRectangleRec(inspector, Color{ 35,35,45,255 });
		DrawText(
			"Inspector",
			static_cast<int>(inspector.x + 15),
			10,
			20,
			WHITE
		);

		if (selectedObject == nullptr)
		{
			DrawText(
				"No object selected",
				static_cast<int>(inspector.x + 15),
				65,
				17,
				LIGHTGRAY
			);
			return;
		}

		int x = static_cast<int>(inspector.x + 15);
		int y = 65;

		DrawText("Name", x, y, 16, LIGHTGRAY);
		DrawText(selectedObject->name.c_str(), x, y + 23, 18, RAYWHITE);

		y += 65;

		DrawText("Tag", x, y, 16, LIGHTGRAY);
		DrawText(selectedObject->tag.c_str(), x, y + 23, 18, RAYWHITE);

		y += 65;

		DrawText("Transform", x, y, 20, GOLD);

		y += 35;

		char positionText[64];							//raylibのDrawTextと下のsnprintf用のchar型の配列を作る　64文字分のバッファを確保する
		std::snprintf(									//snprintfで変数を文字列に変換して、positionTextで渡されたアドレスから順に格納する
			positionText,								//positionTextの先頭アドレスを渡している
			sizeof(positionText),						//positionText[64]のサイズ64を返す　入れる文字列がこれより大きかったら入らなかった部分を切り落としてくれる関数snprintf()
			"Position: %.0f, %.0f",						//%.0fで小数点以下を切り捨てる
			selectedObject->position.x,					//selectedObject->position.xで選択されたオブジェクトのx座標を取得する
			selectedObject->position.y					//selectedObject->position.yで選択されたオブジェクトのy座標を取得する
		);
		DrawText(positionText, x, y, 17, RAYWHITE);

		y += 30;

		char rotationText[64];
		std::snprintf(
			rotationText,
			sizeof(rotationText),
			"Rotation: %.0f",
			selectedObject->rotation
		);
		DrawText(rotationText, x, y, 17, RAYWHITE);

		y += 30;

		char scaleText[64];
		std::snprintf(
			scaleText,
			sizeof(scaleText),
			"Scale: %.1f, %.1f",
			selectedObject->scale.x,
			selectedObject->scale.y
		);
		DrawText(scaleText, x, y, 17, RAYWHITE);

		y += 55;

		DrawText("Components", x, y, 20, GOLD);
		y += 35;

		if (selectedObject->components.empty())
		{
			DrawText("None", x, y, 17, LIGHTGRAY);
		}
		else
		{
			DrawText(
				TextFormat("%d component(s)", static_cast<int>(selectedObject->components.size())),
				x,
				y,
				17,
				RAYWHITE
			);
		}
	}
}

void InitializeEditor()
{
	editorObjects.clear();
	selectedObject = nullptr;
	nextObjectId = 1;

	// 動作確認用の初期オブジェクト
	AddEmptyObject();											//新しい空オブジェクトをeditorObjectsに追加する関数を呼ぶ
	editorObjects.back()->name = "Player";						//editorObjects.back()でeditorObjectsの最後の要素を取得して、そこのnameを"Player"にする
	editorObjects.back()->tag = "Player";
	editorObjects.back()->position = { 500.0f, 300.0f };

	AddEmptyObject();
	editorObjects.back()->name = "Enemy";
	editorObjects.back()->tag = "Enemy";
	editorObjects.back()->position = { 700.0f, 450.0f };
}

void UpdateEditor() {
	// if (IsKeyPressed(KEY_B)) {	//main.cppの方でやってるのでここではやらない
	// 	selectedObject = nullptr;
	// 	isDragging = false;
	// 	return;
	// }

	Vector2 mousePosition = GetMousePosition();

	Rectangle addButton = { 10, 40, 190, 30 };

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		if (CheckCollisionPointRec(mousePosition, addButton)) {
			AddEmptyObject();
			return;
		}

		//Hierarchyから選択する
		int objectY = 100;

		for (auto& obj : editorObjects) {
			Rectangle itemRect = {
				10,
				static_cast<float>(objectY),
				200,
				30
			};

			if (CheckCollisionPointRec(mousePosition, itemRect)) {
				selectedObject = obj.get();
				return;
			}

			objectY += 35;
		}

		//Scene上のオブジェクトを選択
		if (IsPointInScene(mousePosition)) {
			selectedObject = FindObjectAtPosition(mousePosition);
			if (selectedObject != nullptr) {
				isDragging = true;
				dragOffset.x = selectedObject->position.x - mousePosition.x;
				dragOffset.y = selectedObject->position.y - mousePosition.y;
			}
		}
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
		isDragging &&
		selectedObject != nullptr)
	{
		selectedObject->position = {
			mousePosition.x + dragOffset.x,
			mousePosition.y + dragOffset.y
		};
	}

	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		isDragging = false;
	}
}

void DrawEditor()
{
	DrawHierarchy();
	DrawScene();
	DrawInspector();

	DrawText(
		"B: Back to Title",
		15,
		GetScreenHeight() - 25,
		15,
		LIGHTGRAY
	);
}

void ShutdownEditor()
{
	editorObjects.clear();
	selectedObject = nullptr;
}